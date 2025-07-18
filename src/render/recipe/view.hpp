#pragma once

#include "backend/id_types.hpp"
#include "render/common.hpp"
#include "utils/ingredients_availability.hpp"

#include <vector>

namespace cookcookhnya::render::recipe {

struct textGenInfo {
    std::string text;
    bool isIngredientNotAvailable;
    bool isIngredientIsOtherStorages;
};

void renderRecipeView(std::vector<std::pair<api::models::recipe::IngredientInRecipe, utils::IngredientAvailability>>&
                          inStoragesAvailability,
                      api::RecipeId recipeId,
                      UserId userId,
                      ChatId chatId,
                      BotRef bot,
                      ApiClient api);

textGenInfo
recipeView(const std::vector<std::pair<api::models::recipe::IngredientInRecipe, utils::IngredientAvailability>>&
               inStoragesAvailability,
           api::RecipeId recipeId,
           UserId userId,
           ApiClient api);

} // namespace cookcookhnya::render::recipe
