#include "storage_view_members.hpp"

#include "render/storage_list/storage_list_render.hpp"
#include "render/storage_view/storage_members_render.hpp"

namespace cookcookhnya::handlers::storageViewMembers {
using namespace cookcookhnya::forHandlers;
using namespace cookcookhnya::render::viewStorageMembers;
using namespace cookcookhnya::render::storageList;

void storageMemberViewButtonCallback(
    StorageMemberView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, BackendApiRef api) {
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;
    if (cq.data == "add_delete_member") {
        stateManager.put(MembersAdditionDeletion{state.storageId});
        renderMemberAdditionDeletionPrompt(state.storageId, userId, chatId, bot, api);
    } else if (cq.data == "back") {
        stateManager.put(StorageView{state.storageId});
        renderStorageList(userId, chatId, bot, api);
    }
};

} // namespace cookcookhnya::handlers::storageViewMembers
