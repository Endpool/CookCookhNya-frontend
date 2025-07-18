#include "view.hpp"

#include "backend/id_types.hpp"
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
        renderRecipesSuggestion(state.storageIds, 0, userId, chatId, bot, api);
        stateManager.put(
            SuggestedRecipeList{.pageNo = 0, .storageIds = std::move(state.storageIds), .fromStorage = false});
        return;
    }

    if (cq.data == "cancel") {
        renderMainMenu(true, userId, chatId, bot, api);
        stateManager.put(MainMenu{});
        return;
    }

    if (auto storageId = utils::parseSafe<api::StorageId>(cq.data.substr(sizeof("in__") - 1))) {
        if (cq.data.starts_with("in")) {
            if (auto it = std::ranges::find(state.storageIds, *storageId); it != state.storageIds.end())
                state.storageIds.erase(it);
        } else if (cq.data.starts_with("out")) {
            state.storageIds.push_back(*storageId);
        }
        renderStorageSelection(state, userId, chatId, bot, api);
        return;
    }
}
} // namespace cookcookhnya::handlers::storages_selection
