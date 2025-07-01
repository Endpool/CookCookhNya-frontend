#pragma once

#include "backend/id_types.hpp"
#include "render/common.hpp"

#include <vector>

namespace cookcookhnya::render::recipe_view {

void renderRecipeView(const std::vector<api::StorageId>& storageIds,
                      api::RecipeId recipeId,
                      UserId userId,
                      ChatId chatId,
                      BotRef bot,
                      RecipesApiRef recipesApi);
} // namespace cookcookhnya::render::recipe_view
