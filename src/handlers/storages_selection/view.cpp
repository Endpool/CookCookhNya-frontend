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
    auto selectedStorages = state.storages;

    if (cq.data == "confirm") {
        renderRecipesSuggestion(selectedStorages, 0, userId, chatId, bot, api);
        stateManager.put(
            SuggestedRecipeList{.pageNo = 0, .storages = std::move(selectedStorages), .fromStorage = false});
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
            auto it = std::ranges::find(selectedStorages, *storageId, &api::models::storage::StorageSummary::id);
            selectedStorages.erase(it);
            renderStorageSelection(selectedStorages, userId, chatId, bot, api);
            stateManager.put(StoragesSelection{.storages = selectedStorages});
            return;
        }
        if (cq.data[0] == '-') {
            auto storageDetails = api.getStoragesApi().get(userId, *storageId);
            selectedStorages.push_back(
                {.id = *storageId, .name = storageDetails.name, .ownerId = storageDetails.ownerId});
            renderStorageSelection(selectedStorages, userId, chatId, bot, api);
            stateManager.put(StoragesSelection{.storages = selectedStorages});
            return;
        }
    }
}
} // namespace cookcookhnya::handlers::storages_selection
