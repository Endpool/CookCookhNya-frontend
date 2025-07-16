#pragma once

#include "backend/id_types.hpp"
#include "backend/models/recipe.hpp"
#include "backend/models/storage.hpp"
#include "handlers/recipes_suggestions/view.hpp"
#include "render/common.hpp"
#include "render/recipe/view.hpp"

#include <vector>

namespace cookcookhnya::render::recipe {

textGenInfo storageAdditionView(const std::vector<std::pair<api::models::recipe::IngredientInRecipe, handlers::recipes_suggestions::IngredientAvailability>>& inStoragesAvailability,
                    const std::vector<api::models::storage::StorageSummary>& selectedStorages,
                    api::RecipeId recipeId,
                    UserId userId,
                    ApiClient api);

void renderStoragesSuggestion(const std::vector<std::pair<api::models::recipe::IngredientInRecipe, handlers::recipes_suggestions::IngredientAvailability>>& inStoragesAvailability,
                      const std::vector<api::models::storage::StorageSummary>& selectedStorages,
                      api::RecipeId recipeId,
                      UserId userId,
                      ChatId chatId,
                      BotRef bot,
                      ApiClient api);
                    
} // namespace cookcookhnya::render::recipe
