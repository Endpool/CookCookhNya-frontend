#pragma once

#include "backend/api/api.hpp"
#include "backend/models/recipe.hpp"
#include "backend/models/storage.hpp"

#include <cstdint>
#include <vector>

namespace cookcookhnya::utils {

enum struct AvailabiltiyType : std::uint8_t { available, not_available, other_storages };

struct IngredientAvailability {
    AvailabiltiyType available = AvailabiltiyType::not_available;
    std::vector<cookcookhnya::api::models::storage::StorageSummary> storages;
};

std::vector<std::pair<cookcookhnya::api::models::recipe::IngredientInRecipe, IngredientAvailability>>
inStoragesAvailability(std::vector<api::models::storage::StorageSummary>& selectedStorages,
                       cookcookhnya::api::RecipeId recipeId,
                       cookcookhnya::tg_types::UserId userId,
                       const api::ApiClient& api);

} // namespace cookcookhnya::utils
