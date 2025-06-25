#include "recipe_view_render.hpp"
#include <cstddef>
#include <format>
#include <vector>

namespace cookcookhnya::render::recipe_view {

void renderRecipeView(std::vector<StorageId> const& storageIds,
                      api::RecipeId recipeId,
                      UserId userId,
                      ChatId chatId,
                      BotRef bot,
                      RecipesApiRef recipesApi) {

    auto recipeIngredients = recipesApi.getIngredientsInRecipe(userId, recipeId, storageIds);
    auto ingredients = recipeIngredients.ingredients;

    std::string recipeName = recipeIngredients.name;

    std::string toPrint = std::format("Ingredients for {}\n", recipeName);

    for (size_t i = 0; i < ingredients.size(); i++) {
        if (ingredients[i].available) {
            toPrint += std::format("{} +\n", ingredients[i].name);
        } else {
            toPrint += std::format("{} -\n", ingredients[i].name);
        }
    }
    InlineKeyboard keyboard(3);

    keyboard[0].push_back(detail::makeCallbackButton("Готовить", "startCooking")); // Add needed info for next states!
    keyboard[1].push_back(
        detail::makeCallbackButton("Составить список продуктов", "makeReceipt")); // Add needed info for next states!
    keyboard[2].push_back(detail::makeCallbackButton("Назад", "backFromRecipeView"));

    bot.sendMessage(chatId, toPrint, nullptr, nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
}
} // namespace cookcookhnya::render::recipe_view
