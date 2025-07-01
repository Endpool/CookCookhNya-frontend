#include "recipe_view_render.hpp"

#include "backend/id_types.hpp"
#include "render/common.hpp"
#include "utils.hpp"

#include <format>
#include <string>
#include <utility>
#include <vector>

namespace cookcookhnya::render::recipe_view {

void renderRecipeView(const std::vector<api::StorageId>& storageIds,
                      api::RecipeId recipeId,
                      UserId userId,
                      ChatId chatId,
                      BotRef bot,
                      RecipesApiRef recipesApi) {
    auto recipeDetails = recipesApi.getDetails(userId, recipeId, storageIds);
    auto ingredients = recipeDetails.ingredients;

    std::string toPrint = utils::utf8str(u8"Ингредиенты для ") + std::move(recipeDetails.name) + "\n";
    for (auto& ingredient : ingredients) {
        if (ingredient.available)
            toPrint += std::format("{} +\n", std::move(ingredient.name));
        else
            toPrint += std::format("{} -\n", std::move(ingredient.name));
    }

    InlineKeyboard keyboard(3);
    keyboard[0].push_back(detail::makeCallbackButton(u8"Готовить", "startCooking")); // Add needed info for next states!
    keyboard[1].push_back(detail::makeCallbackButton(u8"Составить список продуктов",
                                                     "makeReceipt")); // Add needed info for next states!
    keyboard[2].push_back(detail::makeCallbackButton(u8"Назад", "backFromRecipeView"));

    bot.sendMessage(chatId, toPrint, nullptr, nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
}
} // namespace cookcookhnya::render::recipe_view
