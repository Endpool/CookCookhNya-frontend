#include "view.hpp"

#include "handlers/common.hpp"
#include "render/storage_view/members/add.hpp"
#include "render/storage_view/members/delete.hpp"
#include "render/storage_view/view.hpp"

namespace cookcookhnya::handlers::storage_members {

using namespace render::storage::add_member;
using namespace render::storage::delete_member;
using namespace render::storage;

void storageMemberViewButtonCallback(
    StorageMemberView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi) {
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;
    if (cq.data == "add") {
        renderMemberAdditionPrompt(state.storageId, userId, chatId, bot, storageApi);
        stateManager.put(MemberAddition{state.storageId});
    } else if (cq.data == "delete") {
        renderMemberDeletionPrompt(state.storageId, userId, chatId, bot, storageApi);
        stateManager.put(MemberDeletion{state.storageId});
    } else if (cq.data == "back") {
        renderStorageView(state.storageId, userId, chatId, bot, storageApi);
        stateManager.put(StorageView{state.storageId});
    }
};

} // namespace cookcookhnya::handlers::storage_members
