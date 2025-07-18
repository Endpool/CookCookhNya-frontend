#include "view.hpp"

#include "backend/id_types.hpp"
#include "backend/models/storage.hpp"
#include "handlers/common.hpp"
#include "render/main_menu/view.hpp"
#include "render/recipes_suggestions/view.hpp"
#include "render/storages_selection/view.hpp"
#include "utils/parsing.hpp"

#include <algorithm>
#include <utility>
#include <vector>

namespace cookcookhnya::handlers::storages_selection {

using namespace render::recipes_suggestions;
using namespace render::select_storages;
using namespace render::main_menu;

void handleStoragesSelectionCQ(
    StoragesSelection& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    if (cq.data == "confirm") {
        renderRecipesSuggestion(state.selectedStorages, 0, userId, chatId, bot, api);
        stateManager.put(
            SuggestedRecipeList{.pageNo = 0, .selectedStorages = std::move(state.selectedStorages), .fromStorage = false});
        return;
    }
    if (cq.data == "cancel") {
        renderMainMenu(true, userId, chatId, bot, api);
        stateManager.put(MainMenu{});
        return;
    }

    auto storageId = utils::parseSafe<api::StorageId>(cq.data.substr(1));
    if (storageId) {
        if (cq.data[0] == '+') {
            auto it = std::ranges::find(state.selectedStorages, *storageId, &api::models::storage::StorageSummary::id);
            state.selectedStorages.erase(it);
            renderStorageSelection(state.selectedStorages, userId, chatId, bot, api);
            stateManager.put(StoragesSelection{.selectedStorages = state.selectedStorages});
            return;
        }
        if (cq.data[0] == '-') {
            auto storageDetails = api.getStoragesApi().get(userId, *storageId);
            state.selectedStorages.push_back(
                {.id = *storageId, .name = storageDetails.name});
            renderStorageSelection(state.selectedStorages, userId, chatId, bot, api);
            stateManager.put(StoragesSelection{.selectedStorages = state.selectedStorages});
            return;
        }
    }
}
} // namespace cookcookhnya::handlers::storages_selection
