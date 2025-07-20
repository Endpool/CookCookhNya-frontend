#include "ingredients_availability.hpp"

#include "backend/api/api.hpp"
#include "backend/id_types.hpp"
#include "backend/models/storage.hpp"
#include "states.hpp"
#include "tg_types.hpp"

#include <algorithm>
#include <ranges>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace cookcookhnya::utils {

using namespace api;
using namespace api::models::storage;
using namespace tg_types;
using IngredientAvailability = states::RecipeView::IngredientAvailability;
using AvailabilityType = states::RecipeView::AvailabilityType;
using namespace std::views;
using namespace std::ranges;

std::vector<IngredientAvailability> inStoragesAvailability(std::vector<StorageSummary>& selectedStorages,
                                                           RecipeId recipeId,
                                                           UserId userId,
                                                           const api::ApiClient& api) {
    auto allStorages = api.getStoragesApi().getStoragesList(userId);
    auto recipe = api.getRecipesApi().get(userId, recipeId);

    auto selectedStoragesSet = selectedStorages | views::transform(&StorageSummary::id) | to<std::unordered_set>();

    std::unordered_map<StorageId, StorageSummary> allStoragesMap;
    for (const auto& storage : allStorages) {
        allStoragesMap.emplace(storage.id, storage);
    }

    std::vector<IngredientAvailability> result;

    for (auto& ingredient : recipe.ingredients) {
        IngredientAvailability availability;
        std::vector<StorageSummary> storages;

        bool hasInSelected = false;
        bool hasInOther = false;

        for (auto& storage : ingredient.inStorages) {
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

        availability.ingredient = std::move(ingredient);
        if (hasInSelected) {
            availability.available = AvailabilityType::AVAILABLE;
            availability.storages = std::move(storages);
        } else if (hasInOther) {
            availability.available = AvailabilityType::OTHER_STORAGES;
            availability.storages = std::move(storages);
        } else {
            availability.available = AvailabilityType::NOT_AVAILABLE;
        }

        result.push_back(std::move(availability));
    }

    return result;
}

void addStorage(std::vector<IngredientAvailability>& availability, const StorageSummary& storage) {
    for (auto& info : availability) {
        auto it = std::ranges::find(info.storages, storage.id, &StorageSummary::id);
        if (it != info.storages.end()) {
            info.storages.erase(it);
            info.available = AvailabilityType::AVAILABLE;
        }
    }
}

void deleteStorage(std::vector<IngredientAvailability>& availability, const StorageSummary& storage) {
    for (auto& infoPair : availability) {
        for (auto& storage_ : infoPair.ingredient.inStorages) {
            if (storage.id == storage_.id) {
                infoPair.storages.push_back(storage);
                infoPair.available = AvailabilityType::OTHER_STORAGES;
            }
        }
    }
}

} // namespace cookcookhnya::utils
