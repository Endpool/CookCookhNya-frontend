#pragma once

#include "backend/id_types.hpp"
#include "render/common.hpp"
#include "states.hpp"

#include <string>
#include <vector>

namespace cookcookhnya::render::recipe {

struct textGenInfo {
    std::string text;
    bool isIngredientNotAvailable;
    bool isIngredientIsOtherStorages;
};

void renderRecipeView(std::vector<states::RecipeView::IngredientAvailability>& inStoragesAvailability,
                      api::RecipeId recipeId,
                      UserId userId,
                      ChatId chatId,
                      BotRef bot,
                      ApiClient api);

textGenInfo recipeView(const std::vector<states::RecipeView::IngredientAvailability>& inStoragesAvailability,
                       api::RecipeId recipeId,
                       UserId userId,
                       ApiClient api);

} // namespace cookcookhnya::render::recipe
