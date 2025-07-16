#pragma once

#include "backend/id_types.hpp"
#include "handlers/recipes_suggestions/view.hpp"
#include "render/common.hpp"


#include <vector>

namespace cookcookhnya::render::recipe {

struct textGenInfo {
    std::string text;
    bool isIngredientNotAvailable;
    bool isIngredientIsOtherStorages;
};

void renderRecipeView(const std::vector<std::pair<api::models::recipe::IngredientInRecipe, handlers::recipes_suggestions::IngredientAvailability>>& inStoragesAvailability,
                      api::RecipeId recipeId,
                      UserId userId,
                      ChatId chatId,
                      BotRef bot,
                      ApiClient api);

textGenInfo recipeView(const std::vector<std::pair<api::models::recipe::IngredientInRecipe, handlers::recipes_suggestions::IngredientAvailability>>& inStoragesAvailability,
                    api::RecipeId recipeId,
                    UserId userId,
                    ApiClient api);

} // namespace cookcookhnya::render::recipe
