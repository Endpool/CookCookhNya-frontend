#pragma once

#include "backend/api/ingredients.hpp"
#include "backend/api/recipes.hpp"
#include "backend/api/storages.hpp"
#include "backend/api/users.hpp"
#include "backend/id_types.hpp"
#include "tg_types.hpp"
#include "utils.hpp"

#include <tgbot/Api.h>
#include <tgbot/types/InlineKeyboardButton.h>
#include <tgbot/types/Message.h>

#include <initializer_list>
#include <memory>
#include <string_view>
#include <utility>
#include <vector>

namespace cookcookhnya::render {

// API
using StorageId = api::StorageId;
using UserApiRef = const api::UsersApi&;
using StorageApiRef = const api::StoragesApi&;
using IngredientsApiRef = const api::IngredientsApi&;
using RecipesApiRef = const api::RecipesApi&;

using UserId = tg_types::UserId;
using ChatId = tg_types::ChatId;

using BotRef = const TgBot::Api&;
using MessageRef = const TgBot::Message&;
using InlineKeyboard = std::vector<std::vector<TgBot::InlineKeyboardButton::Ptr>>;

namespace detail {

inline TgBot::InlineKeyboardButton::Ptr makeCallbackButton(std::string_view text, std::string_view data) {
    TgBot::InlineKeyboardButton button{};
    button.text = text;
    button.callbackData = data;
    return utils::make_shared(std::move(button));
}

inline std::vector<TgBot::InlineKeyboardButton::Ptr>
makeButtonRow(std::initializer_list<std::pair<std::string_view, std::string_view>> buttons) {
    std::vector<TgBot::InlineKeyboardButton::Ptr> row;
    row.reserve(buttons.size());
    for (const auto& [text, data] : buttons) {
        row.push_back(makeCallbackButton(text, data));
    }
    return row;
}

inline TgBot::InlineKeyboardMarkup::Ptr makeKeyboardMarkup(InlineKeyboard&& keyboard) {
    auto markup = std::make_shared<TgBot::InlineKeyboardMarkup>();
    markup->inlineKeyboard = std::move(keyboard);
    return markup;
}

} // namespace detail

} // namespace cookcookhnya::render
