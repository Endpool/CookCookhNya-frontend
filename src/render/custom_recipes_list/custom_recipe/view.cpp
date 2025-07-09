#include "view.hpp"
#include "backend/id_types.hpp"
#include "backend/models/recipe.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include <format>
#include <string>

namespace cookcookhnya::render::custom_recipe_view {

void renderCustomRecipe(
    bool toBeEdited, UserId userId, ChatId chatId, api::RecipeId recipeId, BotRef bot, RecipesApiRef recipesApi) {
    // auto recipeDetails = recipesApi.get(userId, recipeId);
    api::models::recipe::CustomRecipeDetails recipeDetailsExample{
        .ingredients = {{{.id = 1, .name = "asd"}, {.id = 2, .name = "asdfg"}}}, .name = "asdgfh", .link = "heweg"};

    std::size_t rows = 4; // 1 for publish, 1 for delete, 1 for back, 1 for change
    InlineKeyboard keyboard(rows);
    std::string toPrint;
    toPrint += (utils::utf8str(u8"Рецепт: ") + recipeDetailsExample.name + "\n");
    for (auto it : recipeDetailsExample.ingredients) {
        toPrint += std::format("• {}\n", it.name);
    }
    toPrint += recipeDetailsExample.link;
    keyboard[0].push_back(detail::makeCallbackButton(u8"Удалить", "delete"));
    keyboard[1].push_back(detail::makeCallbackButton(u8"Редактировать", "change"));
    keyboard[2].push_back(detail::makeCallbackButton(u8"Опубликовать", "publish"));
    keyboard[3].push_back(detail::makeCallbackButton(u8"Назад", "back"));

    if (toBeEdited) {
        auto messageId = message::getMessageId(userId);
        if (messageId) {
            bot.editMessageText(
                toPrint, chatId, *messageId, "", "", nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
        }
    } else {
        auto message =
            bot.sendMessage(chatId, toPrint, nullptr, nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
        message::addMessageId(userId, message->messageId);
    }
}
} // namespace cookcookhnya::render::custom_recipe_view
