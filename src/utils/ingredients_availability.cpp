#include "ingredients_availability.hpp"

#include "backend/id_types.hpp"
#include "backend/models/storage.hpp"

#include <ranges>
#include <unordered_map>

namespace cookcookhnya::utils {

using namespace api;
using namespace tg_types;

std::vector<std::pair<models::recipe::IngredientInRecipe, IngredientAvailability>>
inStoragesAvailability(std::vector<models::storage::StorageSummary>& selectedStorages,
                       RecipeId recipeId,
                       UserId userId,
                       const api::ApiClient& api) {
    auto allStorages = api.getStoragesApi().getStoragesList(userId);
    auto recipe = api.getRecipesApi().get(userId, recipeId);

    auto selectedStoragesSet = selectedStorages | std::views::transform(&api::models::storage::StorageSummary::id) |
                               std::ranges::to<std::unordered_set>();

    std::unordered_map<StorageId, models::storage::StorageSummary> allStoragesMap;
    for (const auto& storage : allStorages) {
        allStoragesMap.emplace(storage.id, storage);
    }

    std::vector<std::pair<models::recipe::IngredientInRecipe, IngredientAvailability>> result;

    for (const auto& ingredient : recipe.ingredients) {
        IngredientAvailability availability;
        std::vector<models::storage::StorageSummary> storages;

        bool hasInSelected = false;
        bool hasInOther = false;

        for (const auto& storage : ingredient.inStorages) {
            auto it = allStoragesMap.find(storage.id);
            if (it == allStoragesMap.end())
                continue;
            if (selectedStoragesSet.contains(storage.id)) {
                storages.push_back(it->second);
                hasInSelected = true;
            } else {
                storages.push_back(it->second);
                hasInOther = true;
            }
        }

        if (hasInSelected) {
            availability.available = AvailabiltiyType::available;
            availability.storages = std::move(storages);
        } else if (hasInOther) {
            availability.available = AvailabiltiyType::other_storages;
            availability.storages = std::move(storages);
        } else {
            availability.available = AvailabiltiyType::not_available;
        }

        result.emplace_back(ingredient, std::move(availability));
    }

    return result;
}

void addStorage(std::vector<std::pair<api::models::recipe::IngredientInRecipe, IngredientAvailability>>& availability,
                const api::models::storage::StorageSummary& storage) {
    for (auto& infoPair : availability) {
        auto it = std::ranges::find(infoPair.second.storages, storage.id, &models::storage::StorageSummary::id);
        if (it != infoPair.second.storages.end()) {
            infoPair.second.storages.erase(it);
            infoPair.second.available = AvailabiltiyType::available;
        }
    }
}

void deleteStorage(
    std::vector<std::pair<api::models::recipe::IngredientInRecipe, IngredientAvailability>>& availability,
    const api::models::storage::StorageSummary& storage) {
    for (auto& infoPair : availability) {
        for (auto& storage_ : infoPair.first.inStorages) {
            if (storage.id == storage_.id) {
                infoPair.second.storages.push_back(storage);
                infoPair.second.available = AvailabiltiyType::other_storages;
            }
        }
    }
}

} // namespace cookcookhnya::utils
