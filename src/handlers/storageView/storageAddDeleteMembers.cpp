#include "storageAddDeleteMembers.hpp"

#include "render/storageView/storageMembersRender.hpp"

namespace cookcookhnya::handlers::storageAddDeleteMembers {
using namespace cookcookhnya::forHandlers;
using namespace cookcookhnya::render::viewStorageMembers;

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
}  // namespace cookcookhnya::handlers::storageAddDeleteMembers