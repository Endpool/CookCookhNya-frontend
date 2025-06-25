#pragma once

#include "backend/id_types.hpp"
#include "render/common.hpp"

namespace cookcookhnya::render::recipe_view {

void renderRecipeView(std::vector<StorageId> const& storageIds,
                      api::RecipeId recipeId,
                      UserId userId,
                      ChatId chatId,
                      BotRef bot,
                      RecipesApiRef recipesApi);
} // namespace cookcookhnya::render::recipe_view
