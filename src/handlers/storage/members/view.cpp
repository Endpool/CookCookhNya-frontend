#include "view.hpp"

#include "backend/api/storages.hpp"
#include "handlers/common.hpp"
#include "render/storage/members/add.hpp"
#include "render/storage/members/delete.hpp"
#include "render/storage/view.hpp"
#include <utility>

namespace cookcookhnya::handlers::storage::members {

using namespace render::storage::members;
using namespace render::storage;

void handleStorageMemberViewCQ(
    StorageMemberView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::StorageApiRef storageApi) {
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;
    if (cq.data == "add") {
        renderStorageMemberAddition(state.storageId, userId, chatId, bot, storageApi);
        stateManager.put(StorageMemberAddition{state.storageId});
    } else if (cq.data == "delete") {
        renderStorageMemberDeletion(state.storageId, userId, chatId, bot, storageApi);
        stateManager.put(StorageMemberDeletion{state.storageId});
    } else if (cq.data == "back") {
        renderStorageView(state.storageId, userId, chatId, bot, storageApi);
        std::string storageName = storageApi.get(userId, state.storageId).name;
        stateManager.put(StorageView{state.storageId, std::move(storageName)});
    }
};

} // namespace cookcookhnya::handlers::storage::members
