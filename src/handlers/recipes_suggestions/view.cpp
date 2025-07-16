#include "view.hpp"

#include "backend/id_types.hpp"
#include "backend/models/recipe.hpp"
#include "backend/models/storage.hpp"
#include "handlers/common.hpp"
#include "render/main_menu/view.hpp"
#include "render/recipe/view.hpp"
#include "render/recipes_suggestions/view.hpp"
#include "render/storage/view.hpp"
#include "render/storages_selection/view.hpp"
#include "utils/parsing.hpp"

#include <string>
#include <unordered_map>
#include <utility>

namespace cookcookhnya::handlers::recipes_suggestions {

using namespace render::recipes_suggestions;
using namespace render::select_storages;
using namespace render::storage;
using namespace render::recipe;
using namespace render::main_menu;

namespace {

using namespace api;

std::vector<std::pair<models::recipe::IngredientInRecipe, IngredientAvailability>> inStoragesAvailability(std::vector<StorageId>& selectedStorages, const RecipeId recipeId, UserId userId, ApiClientRef api){
    auto allStorages = api.getStoragesApi().getStoragesList(userId);
    auto recipe = api.getRecipesApi().getIngredientsInRecipe(userId, recipeId);

    std::unordered_set<StorageId> selectedStoragesSet(selectedStorages.begin(), selectedStorages.end());

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
} // namespace

void handleSuggestedRecipeListCQ(
    SuggestedRecipeList& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    auto data = cq.data;

    if (data == "back") {
        if (state.fromStorage) {
            renderStorageView(state.storageIds[0], cq.from->id, chatId, bot, api);
            stateManager.put(StorageView{state.storageIds[0]}); // Go to the only one storage
        } else {
            if (api.getStoragesApi().getStoragesList(userId).size() == 1) {
                renderMainMenu(true, userId, chatId, bot, api);
                stateManager.put(MainMenu{});
            } else {
                renderStorageSelection(state.storageIds, userId, chatId, bot, api);
                stateManager.put(StoragesSelection{.storageIds = std::move(state.storageIds)});
            }
        }
        bot.answerCallbackQuery(cq.id);
        return;
    }

    if (data[0] == 'r') { // Same naive implementation: if first char is r then it's recipe

        auto recipeId = utils::parseSafe<api::RecipeId>(
            data.substr(data.find(' ', 0) + 1, data.size())); // +1 is to move from space and get pure number
        if (recipeId) {
            auto inStorage = inStoragesAvailability(state.storageIds, *recipeId, userId, api);
            renderRecipeView(inStorage, *recipeId, userId, chatId, bot, api);
            stateManager.put(RecipeView{.storageIds = state.storageIds,
                                        .recipeId = *recipeId,
                                        .fromStorage = state.fromStorage,
                                        .pageNo = state.pageNo});
        }

        return;
    }

    if (data != "dont_handle") {
        auto pageNo = utils::parseSafe<std::size_t>(data);
        if (pageNo) {
            state.pageNo = *pageNo;
        }
        // Message is 100% exists as it was rendered by some another method
        renderRecipesSuggestion(state.storageIds, *pageNo, userId, chatId, bot, api);
        return;
    }
}

} // namespace cookcookhnya::handlers::recipes_suggestions
