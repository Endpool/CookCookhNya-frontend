#include "storage_view_members.hpp"

#include "render/storage_view/storage_members_render.hpp"
#include "render/storage_view/storage_view_render.hpp"

namespace cookcookhnya::handlers::storage_view_members {

using namespace render::storage::member_list;
using namespace render::storage;

void storageMemberViewButtonCallback(
    StorageMemberView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi) {
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;
    if (cq.data == "add_delete_member") {
        renderMemberAdditionDeletionPrompt(state.storageId, userId, chatId, bot, storageApi);
        stateManager.put(MembersAdditionDeletion{state.storageId});
    } else if (cq.data == "back_to_view_storage") {
        renderStorageView(state.storageId, userId, chatId, bot, storageApi);
        stateManager.put(StorageView{state.storageId});
    }
};

} // namespace cookcookhnya::handlers::storage_view_members
