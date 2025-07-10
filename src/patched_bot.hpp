#pragma once

#include "tg_types.hpp"

#include <tgbot/Api.h>
#include <tgbot/types/InlineKeyboardMarkup.h>

#include <cstddef>
#include <string_view>

namespace cookcookhnya {

/**
 *  Changes:
 *  - InlineKeyboardButton does not send "pay" if it is false
 *  - Proper JSON escaping for button texts
 */
class PatchedBot : TgBot::Api {
  public:
    PatchedBot(const TgBot::Api& bot) : Api{bot} {} // NOLINT(*explicit*)

    [[nodiscard]] Api& getUnderlying() {
        return *this;
    }

    [[nodiscard]] const Api& getUnderlying() const {
        return *this;
    }

    TgBot::Message::Ptr sendMessage(tg_types::ChatId chatId, // NOLINT(*nodiscard)
                                    std::string_view text,
                                    const TgBot::InlineKeyboardMarkup::Ptr& replyMarkup) const;

    // For code compatibility
    TgBot::Message::Ptr sendMessage(tg_types::ChatId chatId, // NOLINT(*nodiscard)
                                    std::string_view text,
                                    std::nullptr_t /*unused*/,
                                    std::nullptr_t /*unused*/,
                                    const TgBot::InlineKeyboardMarkup::Ptr& replyMarkup) const {
        return sendMessage(chatId, text, replyMarkup);
    }

    TgBot::Message::Ptr editMessageText(std::string_view text, // NOLINT(*nodiscard)
                                        tg_types::ChatId chatId,
                                        tg_types::MessageId messageId,
                                        const TgBot::InlineKeyboardMarkup::Ptr& replyMarkup) const;

    // For code compatibility
    TgBot::Message::Ptr editMessageText(std::string_view text, // NOLINT(*nodiscard)
                                        tg_types::ChatId chatId,
                                        tg_types::MessageId messageId,
                                        const char* /*unused*/,
                                        const char* /*unused*/,
                                        std::nullptr_t,
                                        const TgBot::InlineKeyboardMarkup::Ptr& replyMarkup) const {
        return editMessageText(text, chatId, messageId, replyMarkup);
    }

    TgBot::Message::Ptr editMessageReplyMarkup(tg_types::ChatId chatId, // NOLINT(*nodiscard*)
                                               tg_types::MessageId messageId,
                                               const TgBot::InlineKeyboardMarkup::Ptr& replyMarkup) const;
};

} // namespace cookcookhnya
