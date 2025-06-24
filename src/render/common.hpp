#pragma once

#include "backend/api/recipes.hpp"
#include "backend/api/storages.hpp"
#include "backend/api/users.hpp"
#include "backend/id_types.hpp"
#include "tg_types.hpp"
#include "utils.hpp"

#include <tgbot/Api.h>
#include <tgbot/types/InlineKeyboardButton.h>
#include <tgbot/types/Message.h>

#include <memory>
#include <string_view>
#include <vector>

namespace cookcookhnya::render {

using namespace tg_types;

// API
using StorageId = api::StorageId;
using StorageApiRef = const api::StoragesApi&;
using UserApiRef = const api::UsersApi&;
using RecipesApiRef = const api::RecipesApi&;

using UserId = tg_types::UserId;
using ChatId = tg_types::ChatId;

using BotRef = const TgBot::Api&;
using MessageRef = const TgBot::Message&;
using InlineKeyboard = std::vector<std::vector<TgBot::InlineKeyboardButton::Ptr>>;

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
