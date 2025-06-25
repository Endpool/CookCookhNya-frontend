#include "storages_select.hpp"

#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/recipes_suggestion/recipes_suggestion_render.hpp"
#include "render/recipes_suggestion/select_storages_render.hpp"
#include "render/storage_list/storage_list_render.hpp"
#include "utils.hpp"

#include <utility>
#include <vector>

namespace cookcookhnya::handlers::storages_select {

using render::recipes_suggestion::renderRecipesSuggestion;
using render::select_storages::updateStorageSelect;
using render::storage_list::renderStorageList;

void selectStorages(StorageSelection& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;
    auto selectedStorages = state.storageIds;
    auto messageId = state.messageId;

    if (cq.data == "confirm_storages_selection") {
        renderRecipesSuggestion(selectedStorages, 1, userId, chatId, bot, api);
        stateManager.put(
            SuggestedRecipeList{.pageNo = 1, .storageIds = std::move(selectedStorages), .fromStorage = false});
        return;
    }
    if (cq.data == "cancel_storages_selection") {
        renderStorageList(userId, chatId, bot, api);
        stateManager.put(StorageList{});
        return;
    }

    auto cqStorageId = utils::parseSafe<api::StorageId>(cq.data.substr(4, cq.data.length() - 4));
    if (cq.data.starts_with("in")) {
        auto it = std::ranges::find(selectedStorages, *cqStorageId);
        selectedStorages.erase(it);
        updateStorageSelect(selectedStorages, messageId, userId, chatId, bot, api);
        stateManager.put(StorageSelection{.storageIds = selectedStorages, .messageId = messageId});
        return;
    }
    if (cq.data.starts_with("out")) {
        selectedStorages.push_back(*cqStorageId);
        updateStorageSelect(selectedStorages, messageId, userId, chatId, bot, api);
        stateManager.put(StorageSelection{.storageIds = selectedStorages, .messageId = messageId});
        return;
    }
}
} // namespace cookcookhnya::handlers::storages_select
