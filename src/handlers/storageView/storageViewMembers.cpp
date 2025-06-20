#include "storageViewMembers.hpp"

#include "../../render/storageList/storageListRender.hpp"
#include "../../render/storageView/storageMembersRender.hpp"

namespace handlers::storageViewMembers {
using namespace cookcookhnya::forHandlers;
using namespace render::viewStorageMembers;
using namespace render::storageList;

void storageMemberViewButtonCallback(StorageMemberView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager,  BackendApiRef api) {
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

}  // namespace handlers::storageViewMembers