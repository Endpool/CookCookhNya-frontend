#include "view.hpp"

#include "backend/api/api.hpp"
#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/cooking_planning/view.hpp"
#include "render/main_menu/view.hpp"
#include "render/recipes_search/view.hpp"
#include "render/storages_selection/view.hpp"
#include "states.hpp"
#include "utils/ingredients_availability.hpp"

#include <optional>
#include <utility>
#include <vector>

namespace cookcookhnya::handlers::recipe {

using namespace render::recipes_search;
using namespace render::cooking_planning;
using namespace render::select_storages;
using namespace render::main_menu;

void handleRecipeViewCQ(RecipeView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    auto userId = cq.from->id;
    auto chatId = cq.message->chat->id;

    if (cq.data == "back") {
        if (auto& prevState = state.prevState) {
            renderRecipesSearch(prevState->pagination, prevState->page, userId, chatId, bot);
            stateManager.put(auto{std::move(*prevState)});
        } else {
            renderMainMenu(true, std::nullopt, userId, chatId, bot, api);
            stateManager.put(MainMenu{});
        }
        return;
    }

    if (cq.data == "cook") {
        auto storages = api.getStoragesApi().getStoragesList(userId);
        if (storages.size() <= 1) {
            std::vector<api::StorageId> storagesIds;
            if (!storages.empty())
                storagesIds.push_back(storages[0].id);

            const api::RecipeId recipeId = state.recipeId;
            auto availability = utils::inStoragesAvailability(storagesIds, recipeId, userId, api);

            renderCookingPlanning(availability, recipeId, userId, chatId, bot, api);
            stateManager.put(
                CookingPlanning{.prevState = CookingPlanning::FromRecipeViewData{std::move(state), std::move(storages)},
                                .addedStorages = {},
                                .availability = std::move(availability),
                                .recipeId = recipeId});
            return;
        }
        StoragesSelection newState{.prevState = std::move(state), .selectedStorages = {}};
        renderStorageSelection(newState, userId, chatId, bot, api);
        stateManager.put(std::move(newState));
        return;
    }
}

} // namespace cookcookhnya::handlers::recipe
