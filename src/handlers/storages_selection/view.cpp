#include "view.hpp"

#include "backend/api/api.hpp"
#include "backend/id_types.hpp"
#include "backend/models/storage.hpp"
#include "handlers/common.hpp"
#include "render/cooking_planning/view.hpp"
#include "render/main_menu/view.hpp"
#include "render/recipe/view.hpp"
#include "render/recipes_suggestions/view.hpp"
#include "render/storages_selection/view.hpp"
#include "utils/ingredients_availability.hpp"
#include "utils/parsing.hpp"

#include <algorithm>
#include <optional>
#include <ranges>
#include <utility>
#include <variant>
#include <vector>

namespace cookcookhnya::handlers::storages_selection {

using api::models::storage::StorageSummary;
using namespace render::recipes_suggestions;
using namespace render::select_storages;
using namespace render::main_menu;
using namespace render::cooking_planning;
using namespace render::recipe;
using namespace std::views;
using std::ranges::to;

void handleStoragesSelectionCQ(
    StoragesSelection& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    if (cq.data == "confirm") {
        auto storagesIds = state.selectedStorages | transform(&StorageSummary::id) | to<std::vector>();
        if (auto* prevState = std::get_if<RecipeView>(&state.prevState)) {
            const api::RecipeId recipeId = prevState->recipeId;
            auto availability = utils::inStoragesAvailability(storagesIds, recipeId, userId, api);

            renderCookingPlanning(availability, recipeId, userId, chatId, bot, api);
            stateManager.put(
                CookingPlanning{.prevState = CookingPlanning::FromRecipeViewData{std::move(*prevState),
                                                                                 std::move(state.selectedStorages)},
                                .addedStorages = {},
                                .availability = std::move(availability),
                                .recipeId = recipeId});
        } else if (std::holds_alternative<MainMenu>(state.prevState)) {
            renderRecipesSuggestion(storagesIds, 0, userId, chatId, bot, api);
            stateManager.put(SuggestedRecipesList{.prevState = std::move(state), .pageNo = 0});
        }
        return;
    }

    if (cq.data == "cancel") {
        if (auto* prevState = std::get_if<MainMenu>(&state.prevState)) {
            renderMainMenu(true, std::nullopt, userId, chatId, bot, api);
            stateManager.put(auto{*prevState});
        } else if (auto* prevState = std::get_if<RecipeView>(&state.prevState)) {
            renderRecipeView(prevState->recipe, prevState->recipeId, userId, chatId, bot);
            stateManager.put(auto{std::move(*prevState)});
        }
        return;
    }

    if (auto mSelectedStorageId = utils::parseSafe<api::StorageId>(cq.data.substr(1))) {
        if (cq.data[0] == '+') {
            auto it = std::ranges::find(state.selectedStorages, *mSelectedStorageId, &StorageSummary::id);
            state.selectedStorages.erase(it);
        } else if (cq.data[0] == '-') {
            auto storageDetails = api.getStoragesApi().get(userId, *mSelectedStorageId);
            state.selectedStorages.push_back({.id = *mSelectedStorageId, .name = storageDetails.name});
        }
        renderStorageSelection(state, userId, chatId, bot, api);
        return;
    }
}
} // namespace cookcookhnya::handlers::storages_selection
