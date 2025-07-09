#include "patched_bot.hpp"

#include "tg_types.hpp"

#include <boost/json/serialize.hpp>
#include <boost/json/string.hpp>
#include <tgbot/net/HttpReqArg.h>
#include <tgbot/types/InlineKeyboardButton.h>
#include <tgbot/types/InlineKeyboardMarkup.h>
#include <tgbot/types/Message.h>

#include <string>
#include <string_view>
#include <vector>

namespace cookcookhnya {

namespace {

std::string escapeString(std::string_view s) {
    return boost::json::serialize(boost::json::string(s));
}

void removeLastComma(std::string& input) {
    input.erase(input.length() - 1);
}

void appendToJson(std::string& json, std::string_view varName, std::string_view value) {
    json += escapeString(varName);
    json += ':';
    if (value.empty())
        json += R"("")";
    else
        json += escapeString(value);
    json += ',';
}

[[nodiscard]] std::string parseInlineKeyboardButton(const TgBot::InlineKeyboardButton::Ptr& object) {
    if (!object)
        return "";
    std::string result;
    result += '{';
    appendToJson(result, "text", object->text);
    if (!object->callbackData.empty())
        appendToJson(result, "callback_data", object->callbackData);
    else
        appendToJson(result, "switch_inline_query_current_chat", object->switchInlineQueryCurrentChat);
    removeLastComma(result);
    result += '}';
    return result;
}

[[nodiscard]] std::string parseInlineKeyboardMarkup(const TgBot::InlineKeyboardMarkup::Ptr& object) {
    if (!object)
        return "";
    std::string result;
    result += '{';
    result += R"("inline_keyboard":[)";
    for (const auto& row : object->inlineKeyboard) {
        result += '[';
        for (const auto& innerItem : row) {
            result += parseInlineKeyboardButton(innerItem);
            result += ',';
        }
        removeLastComma(result);
        result += "],";
    }
    if (!object->inlineKeyboard.empty())
        removeLastComma(result);
    result += "]}";
    return result;
}

} // namespace

TgBot::Message::Ptr // NOLINT(*nodiscard)
PatchedBot::sendMessage(tg_types::ChatId chatId,
                        std::string_view text,
                        const TgBot::InlineKeyboardMarkup::Ptr& replyMarkup) const {
    std::vector<TgBot::HttpReqArg> args;
    args.reserve(3);
    args.emplace_back("chat_id", chatId);
    args.emplace_back("text", text);
    if (replyMarkup)
        args.emplace_back("reply_markup", parseInlineKeyboardMarkup(replyMarkup));
    return _tgTypeParser.parseJsonAndGetMessage(sendRequest("sendMessage", args));
}

TgBot::Message::Ptr // NOLINT(*nodiscard)
PatchedBot::editMessageText(std::string_view text,
                            tg_types::ChatId chatId,
                            tg_types::MessageId messageId,
                            const TgBot::InlineKeyboardMarkup::Ptr& replyMarkup) const {
    std::vector<TgBot::HttpReqArg> args;
    args.reserve(4);
    args.emplace_back("text", text);
    args.emplace_back("chat_id", chatId);
    args.emplace_back("message_id", messageId);
    if (replyMarkup) {
        args.emplace_back("reply_markup", parseInlineKeyboardMarkup(replyMarkup));
    }
    return _tgTypeParser.parseJsonAndGetMessage(sendRequest("editMessageText", args));
}

TgBot::Message::Ptr PatchedBot::editMessageReplyMarkup(tg_types::ChatId chatId, // NOLINT(*nodiscard*)
                                                       tg_types::MessageId messageId,
                                                       const TgBot::InlineKeyboardMarkup::Ptr& replyMarkup) const {
    std::vector<TgBot::HttpReqArg> args;
    args.reserve(3);
    args.emplace_back("chat_id", chatId);
    args.emplace_back("message_id", messageId);
    if (replyMarkup)
        args.emplace_back("reply_markup", parseInlineKeyboardMarkup(replyMarkup));
    return _tgTypeParser.parseJsonAndGetMessage(sendRequest("editMessageReplyMarkup", args));
}

} // namespace cookcookhnya
