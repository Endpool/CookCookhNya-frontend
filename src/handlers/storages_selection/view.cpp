#include "view.hpp"

#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/main_menu/view.hpp"
#include "render/recipes_suggestions/view.hpp"
#include "render/storages_selection/view.hpp"
#include "utils.hpp"

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
    auto selectedStorages = state.storageIds;

    if (cq.data == "confirm") {
        editRecipesSuggestion(selectedStorages, 0, userId, chatId, bot, api);
        stateManager.put(
            SuggestedRecipeList{.pageNo = 0, .storageIds = std::move(selectedStorages), .fromStorage = false});
        return;
    }
    if (cq.data == "cancel") {
        renderMainMenu(true, userId, chatId, bot, api);
        stateManager.put(MainMenu{});
        return;
    }

    auto storageId = utils::parseSafe<api::StorageId>(cq.data.substr(4));
    if (storageId) {
        if (cq.data.starts_with("in")) {
            auto it = std::ranges::find(selectedStorages, *storageId);
            selectedStorages.erase(it);
            renderStorageSelect(selectedStorages, userId, chatId, bot, api);
            stateManager.put(StoragesSelection{.storageIds = selectedStorages});
            return;
        }
        if (cq.data.starts_with("out")) {
            selectedStorages.push_back(*storageId);
            renderStorageSelect(selectedStorages, userId, chatId, bot, api);
            stateManager.put(StoragesSelection{.storageIds = selectedStorages});
            return;
        }
    }
}
} // namespace cookcookhnya::handlers::storages_selection
