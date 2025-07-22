#pragma once

#include "backend/api/api.hpp"
#include "backend/id_types.hpp"
#include "render/common.hpp"
#include "states.hpp"

#include <string>
#include <vector>

namespace cookcookhnya::render::recipe {

struct TextGenInfo {
    std::string text;
    bool isIngredientNotAvailable;
    bool isIngredientIsOtherStorages;
};

void renderRecipeView(std::vector<states::RecipeView::IngredientAvailability>& inStoragesAvailability,
                      api::RecipeId recipeId,
                      UserId userId,
                      ChatId chatId,
                      BotRef bot,
                      api::ApiClientRef api);

TextGenInfo recipeView(const std::vector<states::RecipeView::IngredientAvailability>& inStoragesAvailability,
                       api::RecipeId recipeId,
                       UserId userId,
                       api::ApiClientRef api);

} // namespace cookcookhnya::render::recipe
