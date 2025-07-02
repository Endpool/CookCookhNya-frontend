#pragma once

#include "render/common.hpp"
#include "tg_types.hpp"

#include <tgbot/Api.h>
#include <tgbot/TgTypeParser.h>
#include <tgbot/net/HttpReqArg.h>
#include <tgbot/types/InlineKeyboardMarkup.h>

#include <string>
#include <string_view>
#include <vector>

namespace cookcookhnya::render {

namespace { // NOLINT(*-header-anon-*,*)

class PatchedBot : TgBot::Api {
    static void removeLastComma(std::string& input) {
        input.erase(input.length() - 1);
    }

    static void appendToJson(std::string& json, const std::string& varName, const std::string& value) {
        json += '"';
        json += varName;
        json += R"(":)";
        if (value.empty()) {
            json += R"("")";
        } else {
            if (value.front() != '{') {
                json += '"';
            }
            json += value;
            if (value.back() != '}') {
                json += '"';
            }
        }
        json += ',';
    }

    [[nodiscard]] static std::string parseInlineKeyboardButton(const TgBot::InlineKeyboardButton::Ptr& object) {
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

    [[nodiscard]] static std::string parseInlineKeyboardMarkup(const TgBot::InlineKeyboardMarkup::Ptr& object) {
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

  public:
    explicit PatchedBot(const TgBot::Api& bot) : Api{bot} {}

    // A special copy of the `sendMessage` to avoid passing `"pay": false` to the request
    [[nodiscard]] TgBot::Message::Ptr
    sendMessage(ChatId chatId, std::string_view text, const TgBot::InlineKeyboardMarkup::Ptr& replyMarkup) const {
        std::vector<TgBot::HttpReqArg> args;
        args.reserve(3);
        args.emplace_back("chat_id", chatId);
        args.emplace_back("text", text);
        if (replyMarkup)
            args.emplace_back("reply_markup", parseInlineKeyboardMarkup(replyMarkup));
        return _tgTypeParser.parseJsonAndGetMessage(sendRequest("sendMessage", args));
    }

    TgBot::Message::Ptr editMessageReplyMarkup(ChatId chatId, // NOLINT(*nodiscard*)
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
};

} // namespace

} // namespace cookcookhnya::render
