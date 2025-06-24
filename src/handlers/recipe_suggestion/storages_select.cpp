#include "storages_select.hpp"

#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/recipe_suggestion/select_storages_render.hpp"
#include "render/storage_list/storage_list_render.hpp"
#include "states.hpp"
#include "utils.hpp"
#include <iostream>
#include <vector>

namespace cookcookhnya::handlers::storages_select {

using render::select_storages::updateStorageSelect;
using render::storage_list::renderStorageList;

void selectStorages(StorageSelection& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi){
    std::cerr << "in handler/storages_select.cpp\n";
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;
    auto selected_storages = state.storageIds;
    auto messageId = state.messageId;
    if (cq.data == "confirm_storages_selection"){
        // renderRecipesSuggetion();
        stateManager.put(SuggestedRecipeList{selected_storages});
        return;
    }
    if (cq.data == "cancel_storages_selection"){
        renderStorageList(userId, chatId, bot, storageApi);
        stateManager.put(StorageList{});
        return;
    }
    auto cqStorageId = utils::parseSafe<api::StorageId>(cq.data.substr(3, cq.data.length() - 3));
    if (cq.data.starts_with("in")){
        auto it = std::ranges::find(selected_storages, *cqStorageId);
        selected_storages.erase(it);
        updateStorageSelect(selected_storages, messageId, userId, chatId, bot, storageApi);
        stateManager.put(StorageSelection{.storageIds=selected_storages, .messageId=messageId});
        return;
    }
    if (cq.data.starts_with("out")){
        selected_storages.push_back(*cqStorageId);
        updateStorageSelect(selected_storages, messageId, userId, chatId, bot, storageApi);
        stateManager.put(StorageSelection{.storageIds=selected_storages, .messageId=messageId});
        return;
    }
}
} // namespace cookcookhnya::handlers::storages_select
