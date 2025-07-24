#pragma once

#include "backend/api/api.hpp"
#include "backend/id_types.hpp"
#include "backend/models/storage.hpp"
#include "render/common.hpp"
#include "states.hpp"

#include <vector>

namespace cookcookhnya::render::cooking_planning {

void renderStoragesSuggestion(
    const std::vector<states::CookingPlanning::IngredientAvailability>& inStoragesAvailability,
    const std::vector<api::models::storage::StorageSummary>& selectedStorages,
    const std::vector<api::models::storage::StorageSummary>& addedStorages,
    api::RecipeId recipeId,
    UserId userId,
    ChatId chatId,
    BotRef bot,
    api::ApiClientRef api);

} // namespace cookcookhnya::render::cooking_planning
