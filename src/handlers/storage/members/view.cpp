#include "view.hpp"

#include "handlers/common.hpp"
#include "render/storage/members/add.hpp"
#include "render/storage/members/delete.hpp"
#include "render/storage/view.hpp"

namespace cookcookhnya::handlers::storage::members {

using namespace render::storage::members;
using namespace render::storage;

void storageMemberViewButtonCallback(
    StorageMemberView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi) {
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;
    if (cq.data == "add") {
        renderStorageMemberAdditionPrompt(state.storageId, userId, chatId, bot, storageApi);
        stateManager.put(StorageMemberAddition{state.storageId});
    } else if (cq.data == "delete") {
        renderStorageMemberDeletionPrompt(state.storageId, userId, chatId, bot, storageApi);
        stateManager.put(StorageMemberDeletion{state.storageId});
    } else if (cq.data == "back") {
        renderStorageView(state.storageId, userId, chatId, bot, storageApi);
        stateManager.put(StorageView{state.storageId});
    }
};

} // namespace cookcookhnya::handlers::storage::members
