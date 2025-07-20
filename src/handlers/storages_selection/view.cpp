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

using api::models::storage::StorageSummary;
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
        stateManager.put(SuggestedRecipesList{
            .selectedStorages = std::move(state.selectedStorages), .pageNo = 0, .fromStorage = false});
        return;
    }

    if (cq.data == "cancel") {
        renderMainMenu(true, std::nullopt, userId, chatId, bot, api);
        stateManager.put(MainMenu{});
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
