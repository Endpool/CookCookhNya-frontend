#pragma once

#include "backend/api/api.hpp"
#include "backend/api/ingredients.hpp"
#include "backend/api/recipes.hpp"
#include "backend/api/shopping_lists.hpp"
#include "backend/api/storages.hpp"
#include "backend/api/users.hpp"
#include "patched_bot.hpp"
#include "tg_types.hpp"
#include "utils/utils.hpp"

#include <cstddef>
#include <initializer_list>
#include <memory>
#include <string_view>
#include <utility>
#include <vector>

// Forward declarations
namespace TgBot {
class Message;
class InlineKeyboardButton;
class InlineKeyboardMarkup;
} // namespace TgBot

namespace cookcookhnya::render {

// API
using ApiClient = const api::ApiClient&;
using StorageApiRef = const api::StoragesApi&;
using IngredientsApiRef = const api::IngredientsApi&;
using UserApiRef = const api::UsersApi&;
using RecipesApiRef = const api::RecipesApi&;
using ShoppingListApiRef = const api::ShoppingListApi&;

using UserId = tg_types::UserId;
using ChatId = tg_types::ChatId;
using MessageId = tg_types::MessageId;

using BotRef = const PatchedBot&;
using MessageRef = const TgBot::Message&;
using InlineKeyboard = std::vector<std::vector<std::shared_ptr<TgBot::InlineKeyboardButton>>>;

inline TgBot::InlineKeyboardButton::Ptr makeCallbackButton(std::string_view text, std::string_view data) {
    TgBot::InlineKeyboardButton::Ptr button = std::make_shared<TgBot::InlineKeyboardButton>();
    button->text = text;
    button->callbackData = data;
    return button;
}

inline TgBot::InlineKeyboardButton::Ptr makeCallbackButton(std::u8string_view text, std::string_view data) {
    TgBot::InlineKeyboardButton::Ptr button = std::make_shared<TgBot::InlineKeyboardButton>();
    button->text = utils::utf8str(text);
    button->callbackData = data;
    return button;
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

inline std::shared_ptr<TgBot::InlineKeyboardMarkup> makeKeyboardMarkup(InlineKeyboard&& keyboard) {
    auto markup = std::make_shared<TgBot::InlineKeyboardMarkup>();
    markup->inlineKeyboard = std::move(keyboard);
    return markup;
}

struct NewRow {};

class InlineKeyboardBuilder {
    InlineKeyboard keyboard;

  public:
    explicit InlineKeyboardBuilder(std::size_t reserve = 0) {
        keyboard.reserve(reserve);
        keyboard.emplace_back();
    }

    InlineKeyboardBuilder& operator<<(TgBot::InlineKeyboardButton::Ptr&& button) {
        keyboard.back().push_back(std::move(button));
        return *this;
    }

    void reserveInRow(std::size_t capacity) {
        keyboard.back().reserve(capacity);
    }

    InlineKeyboardBuilder& operator<<(NewRow /*tag*/) {
        keyboard.emplace_back();
        return *this;
    }

    void operator++(int) {
        keyboard.emplace_back();
    }

    void removeRowIfEmpty() {
        if (keyboard.back().empty())
            keyboard.pop_back();
    }

    TgBot::InlineKeyboardMarkup::Ptr build() && {
        return makeKeyboardMarkup(std::move(keyboard));
    }

    operator TgBot::InlineKeyboardMarkup::Ptr() && { // NOLINT(*explicit*)
        return makeKeyboardMarkup(std::move(keyboard));
    }
};

} // namespace cookcookhnya::render
