#pragma once

#include "backend/api/api.hpp"
#include "backend/id_types.hpp"
#include "backend/models/storage.hpp"
#include "states.hpp"
#include "tg_types.hpp"

#include <vector>

namespace cookcookhnya::utils {

std::vector<states::RecipeView::IngredientAvailability>
inStoragesAvailability(std::vector<api::models::storage::StorageSummary>& selectedStorages,
                       api::RecipeId recipeId,
                       tg_types::UserId userId,
                       const api::ApiClient& api);

void addStorage(std::vector<states::RecipeView::IngredientAvailability>& availability,
                const api::models::storage::StorageSummary& storage);

void deleteStorage(std::vector<states::RecipeView::IngredientAvailability>& availability,
                   const api::models::storage::StorageSummary& storage);

} // namespace cookcookhnya::utils
