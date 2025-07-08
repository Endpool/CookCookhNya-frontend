#include "select.hpp"

#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/main_menu/view.hpp"
#include "render/recipes_suggestion/storage_selection/select.hpp"
#include "render/recipes_suggestion/suggest.hpp"
#include "utils.hpp"

#include <algorithm>
#include <utility>
#include <vector>

namespace cookcookhnya::handlers::storages_select {

using namespace render::recipes_suggestion;
using namespace render::select_storages;
using namespace render::main_menu;

void selectStorages(StorageSelection& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
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
            editStorageSelect(selectedStorages, userId, chatId, bot, api);
            stateManager.put(StorageSelection{.storageIds = selectedStorages});
            return;
        }
        if (cq.data.starts_with("out")) {
            selectedStorages.push_back(*storageId);
            editStorageSelect(selectedStorages, userId, chatId, bot, api);
            stateManager.put(StorageSelection{.storageIds = selectedStorages});
            return;
        }
    }
}
} // namespace cookcookhnya::handlers::storages_select
