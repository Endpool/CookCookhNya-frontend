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

void addDeleteMember(MembersAdditionDeletion& state, MessageRef m, BotRef bot, SMRef stateManager, BackendApiRef api) {
    auto chatId = m.chat->id;
    auto userId = m.from->id;
    auto memberId = cookcookhnya::utils::parseSafe<UserId>(m.text.data());
    auto storage = api.get(userId, state.storageId);
    if (!memberId) {
        bot.sendMessage(chatId, "Invalid user ID");
        renderMemberAdditionDeletionPrompt(state.storageId, userId, chatId, bot, api);
        return;
    }
    auto members = api.getStorageMembers(userId, state.storageId);
    bool isMemberof = std::ranges::find(members, *memberId) != members.end();
    if (isMemberof){
        api.deleteMember(userId, state.storageId, *memberId);
        bot.sendMessage(chatId, "Member deleted successfully");
    } else {
        api.addMember(userId, state.storageId, *memberId);
        bot.sendMessage(chatId, "Member added successfully");
    }
    stateManager.put(StorageMemberView{state.storageId});
    renderMemberList(state.storageId, userId, chatId, bot, api);
};

void cancelAddDeleteMember(MembersAdditionDeletion& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, BackendApiRef api) {
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;
    if (cq.data == "member_add_delete_cancel") {
        stateManager.put(StorageMemberView{state.storageId});
        renderMemberList(state.storageId, userId, chatId, bot, api);
    }
};

}  // namespace handlers::storageViewMembers