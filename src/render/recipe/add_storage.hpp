#pragma once

#include "backend/id_types.hpp"
#include "backend/models/storage.hpp"
#include "render/common.hpp"
#include "render/recipe/view.hpp"
#include "utils/ingredients_availability.hpp"

#include <vector>

namespace cookcookhnya::render::recipe {

textGenInfo storageAdditionView(
    const std::vector<std::pair<api::models::recipe::IngredientInRecipe, utils::IngredientAvailability>>&
        inStoragesAvailability,
    const std::vector<api::models::storage::StorageSummary>& selectedStorages,
    api::RecipeId recipeId,
    UserId userId,
    ApiClient api);

void renderStoragesSuggestion(
    const std::vector<std::pair<api::models::recipe::IngredientInRecipe, utils::IngredientAvailability>>&
        inStoragesAvailability,
    const std::vector<api::models::storage::StorageSummary>& selectedStorages,
    const std::vector<api::models::storage::StorageSummary>& addedStorages,
    api::RecipeId recipeId,
    UserId userId,
    ChatId chatId,
    BotRef bot,
    ApiClient api);

} // namespace cookcookhnya::render::recipe
