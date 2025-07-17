#include "ingredients_availability.hpp"

#include "backend/id_types.hpp"

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
    auto recipe = api.getRecipesApi().getIngredientsInRecipe(userId, recipeId);

    auto selectedStoragesSet = selectedStorages | std::views::transform(&api::models::storage::StorageSummary::id) |
                               std::ranges::to<std::unordered_set>();

    std::unordered_map<StorageId, models::storage::StorageSummary> allStoragesMap;
    for (const auto& storage : allStorages) {
        allStoragesMap.emplace(storage.id, storage);
    }

    std::vector<std::pair<models::recipe::IngredientInRecipe, IngredientAvailability>> result;

    for (const auto& ingredient : recipe.ingredients) {
        IngredientAvailability availability;
        std::vector<models::storage::StorageSummary> otherStorages;

        bool hasInSelected = false;

        for (const auto& storageId : ingredient.inStorages) {
            auto it = allStoragesMap.find(storageId);
            if (it == allStoragesMap.end())
                continue;
            if (selectedStoragesSet.contains(storageId)) {
                hasInSelected = true;
            } else {
                otherStorages.push_back(it->second);
            }
        }

        if (hasInSelected) {
            availability.available = AvailabiltiyType::available;
        } else if (!otherStorages.empty()) {
            availability.available = AvailabiltiyType::other_storages;
            availability.storages = std::move(otherStorages);
        } else {
            availability.available = AvailabiltiyType::not_available;
        }

        result.emplace_back(ingredient, std::move(availability));
    }

    return result;
}
} // namespace cookcookhnya::utils
