#pragma once

#include "backend/api/api.hpp"
#include "backend/id_types.hpp"
#include "backend/models/recipe.hpp"
#include "backend/models/storage.hpp"

#include <cstdint>
#include <vector>

namespace cookcookhnya::utils {

enum struct AvailabiltiyType : std::uint8_t { available, not_available, other_storages };

struct IngredientAvailability {
    AvailabiltiyType available = AvailabiltiyType::not_available;
    std::vector<api::models::storage::StorageSummary> storages;
};

std::vector<std::pair<api::models::recipe::IngredientInRecipe, IngredientAvailability>>
inStoragesAvailability(std::vector<api::models::storage::StorageSummary>& selectedStorages,
                       api::RecipeId recipeId,
                       tg_types::UserId userId,
                       const api::ApiClient& api);

void addStorage(std::vector<std::pair<api::models::recipe::IngredientInRecipe, IngredientAvailability>>& availability,
                const api::models::storage::StorageSummary& storage);

void deleteStorage(
    std::vector<std::pair<api::models::recipe::IngredientInRecipe, IngredientAvailability>>& availability,
    const api::models::storage::StorageSummary& storage);

} // namespace cookcookhnya::utils
