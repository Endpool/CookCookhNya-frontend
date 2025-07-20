#include "ingredients_availability.hpp"

#include "backend/api/api.hpp"
#include "backend/id_types.hpp"
#include "backend/models/recipe.hpp"
#include "backend/models/storage.hpp"
#include "tg_types.hpp"

#include <algorithm>
#include <ranges>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace cookcookhnya::utils {

using namespace api;
using namespace api::models::recipe;
using namespace api::models::storage;
using namespace tg_types;
using namespace std::views;
using namespace std::ranges;

std::vector<std::pair<IngredientInRecipe, IngredientAvailability>> inStoragesAvailability(
    std::vector<StorageSummary>& selectedStorages, RecipeId recipeId, UserId userId, const api::ApiClient& api) {
    auto allStorages = api.getStoragesApi().getStoragesList(userId);
    auto recipe = api.getRecipesApi().get(userId, recipeId);

    auto selectedStoragesSet = selectedStorages | views::transform(&StorageSummary::id) | to<std::unordered_set>();

    std::unordered_map<StorageId, StorageSummary> allStoragesMap;
    for (const auto& storage : allStorages) {
        allStoragesMap.emplace(storage.id, storage);
    }

    std::vector<std::pair<IngredientInRecipe, IngredientAvailability>> result;

    for (const auto& ingredient : recipe.ingredients) {
        IngredientAvailability availability;
        std::vector<StorageSummary> storages;

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
            availability.available = AvailabiltiyType::AVAILABLE;
            availability.storages = std::move(storages);
        } else if (hasInOther) {
            availability.available = AvailabiltiyType::OTHER_STORAGES;
            availability.storages = std::move(storages);
        } else {
            availability.available = AvailabiltiyType::NOT_AVAILABLE;
        }

        result.emplace_back(ingredient, std::move(availability));
    }

    return result;
}

void addStorage(std::vector<std::pair<IngredientInRecipe, IngredientAvailability>>& availability,
                const StorageSummary& storage) {
    for (auto& infoPair : availability) {
        auto it = std::ranges::find(infoPair.second.storages, storage.id, &StorageSummary::id);
        if (it != infoPair.second.storages.end()) {
            infoPair.second.storages.erase(it);
            infoPair.second.available = AvailabiltiyType::AVAILABLE;
        }
    }
}

void deleteStorage(std::vector<std::pair<IngredientInRecipe, IngredientAvailability>>& availability,
                   const StorageSummary& storage) {
    for (auto& infoPair : availability) {
        for (auto& storage_ : infoPair.first.inStorages) {
            if (storage.id == storage_.id) {
                infoPair.second.storages.push_back(storage);
                infoPair.second.available = AvailabiltiyType::OTHER_STORAGES;
            }
        }
    }
}

} // namespace cookcookhnya::utils
