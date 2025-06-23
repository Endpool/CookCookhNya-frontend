#pragma once

#include "backend/api/users.hpp"

#include "backend/id_types.hpp"
#include "utils.hpp"

#include <backend/api/storages.hpp>
#include <tgbot/Api.h>
#include <tgbot/types/CallbackQuery.h>
#include <tgbot/types/InlineKeyboardButton.h>
#include <tgbot/types/Message.h>
#include <tgbot/types/User.h>


namespace cookcookhnya::render {

using StorageId = api::StorageId;

using namespace TgBot;
using namespace cookcookhnya::api;

using InlineKeyboard = std::vector<std::vector<TgBot::InlineKeyboardButton::Ptr>>;
using MessageRef = const Message&;
using BotRef = const Api&;
using StorageApiRef = const StoragesApi&;
using UserApiRef = const UsersApi&;
using UserId = tg_stater::UserIdT;
using ChatId = tg_stater::ChatIdT;
using MessageId = decltype(TgBot::Message::messageId);


namespace detail {
inline std::shared_ptr<TgBot::InlineKeyboardButton> makeCallbackButton(std::string_view text, std::string_view data) {
    TgBot::InlineKeyboardButton button{};
    button.text = text;
    button.callbackData = data;
    return utils::make_shared(std::move(button));
}

inline std::shared_ptr<TgBot::InlineKeyboardMarkup> makeKeyboardMarkup(InlineKeyboard&& keyboard) {
    auto markup = std::make_shared<TgBot::InlineKeyboardMarkup>();
    markup->inlineKeyboard = std::move(keyboard);
    return markup;
}

} // namespace detail
} // namespace cookcookhnya::render

