#pragma once

#include "backend/api/api.hpp"
#include "backend/id_types.hpp"
#include "backend/models/storage.hpp"
#include "states.hpp"
#include "tg_types.hpp"

#include <vector>

namespace cookcookhnya::utils {

std::vector<states::CookingPlanning::IngredientAvailability>
inStoragesAvailability(const std::vector<api::StorageId>& selectedStorages,
                       api::RecipeId recipeId,
                       tg_types::UserId userId,
                       const api::ApiClient& api);

void addStorage(std::vector<states::CookingPlanning::IngredientAvailability>& availability,
                const api::models::storage::StorageSummary& storage);

void deleteStorage(std::vector<states::CookingPlanning::IngredientAvailability>& availability,
                   const api::models::storage::StorageSummary& storage);

} // namespace cookcookhnya::utils
