#include "edit.hpp"

#include "render/common.hpp"

#include <ranges>
#include <string>
#include <utility>

namespace cookcookhnya::render::storage::ingredients {

using namespace api::models::ingredient;

void renderIngredientsEdit(ChatId chat, BotRef bot, IngredientsApiRef api) {
    using namespace std::views;
    std::vector<Ingredient> ingredients = api.getAllIngredients();
    InlineKeyboard keyboard{1 + ingredients.size()};
    keyboard[0].push_back(detail::makeCallbackButton("Back", "back"));
    for (auto [row, i] : zip(drop(keyboard, 1), ingredients)) {
        row.reserve(2);
        row.push_back(detail::makeCallbackButton("Put " + i.name, '+' + std::to_string(i.id)));
        row.push_back(detail::makeCallbackButton("Remove " + std::move(i.name), '-' + std::to_string(i.id)));
    }
    bot.sendMessage(chat,
                    "Use button below to start Google-like search",
                    nullptr,
                    nullptr,
                    detail::makeKeyboardMarkup(std::move(keyboard)));
}

} // namespace cookcookhnya::render::storage::ingredients
