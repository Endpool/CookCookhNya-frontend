#include "search.hpp"

#include "backend/models/ingredient.hpp"
#include "render/common.hpp"
#include "tg_types.hpp"

#include <ranges>
#include <string>
#include <utility>
#include <vector>

namespace cookcookhnya::render::storage::ingredients {

using namespace api::models::ingredient;
using namespace tg_types;

namespace {

auto makeKeyboard(IngredientsApiRef api) {
    using namespace std::views;
    std::vector<Ingredient> ingredients = api.getAllIngredients();
    InlineKeyboard keyboard{1 + ingredients.size()};
    for (auto [row, i] : zip(keyboard, ingredients)) {
        row.reserve(2);
        row.push_back(detail::makeCallbackButton("Put " + i.name, '+' + std::to_string(i.id)));
        row.push_back(detail::makeCallbackButton("Remove " + std::move(i.name), '-' + std::to_string(i.id)));
    }
    keyboard[ingredients.size()].push_back(detail::makeCallbackButton("Back", "back"));
    return detail::makeKeyboardMarkup(std::move(keyboard));
}

} // namespace

MessageId renderStorageIngredientsSearchSend(ChatId chat, BotRef bot, IngredientsApiRef api) {
    return bot.sendMessage(chat, "Use button below to start Google-like search", nullptr, nullptr, makeKeyboard(api))
        ->messageId;
}

// void renderStorageIngredientsSearchEdit(MessageId message, ChatId chat, BotRef bot, IngredientsApiRef api) {
void renderStorageIngredientsSearchEdit(MessageId /**/, ChatId /**/, BotRef /**/, IngredientsApiRef /**/) {
    // bot.editMessageReplyMarkup(chat, message, "", makeKeyboard(api));
}

} // namespace cookcookhnya::render::storage::ingredients
