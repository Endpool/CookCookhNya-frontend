#include "storage_add_delete_members.hpp"

#include "handlers/type_refs.hpp"
#include "render/storage_view/storage_members_render.hpp"

namespace cookcookhnya::handlers::storageAddDeleteMembers {
using namespace cookcookhnya::handlers;
using namespace cookcookhnya::render::viewStorageMembers;

void addDeleteMember(
    MembersAdditionDeletion& state, MessageRef m, BotRef bot, SMRef stateManager, StorageApiRef storageApi) {
    auto chatId = m.chat->id;
    auto userId = m.from->id;
    auto memberId = cookcookhnya::utils::parseSafe<UserId>(m.text.data());
    auto storage = storageApi.get(userId, state.storageId);
    if (!memberId) {
        bot.sendMessage(chatId, "Invalid user ID");
        renderMemberAdditionDeletionPrompt(state.storageId, userId, chatId, bot, storageApi);
        return;
    }
    auto members = storageApi.getStorageMembers(userId, state.storageId);
    bool isMemberof = std::ranges::find(members, *memberId) != members.end();
    if (isMemberof) {
        storageApi.deleteMember(userId, state.storageId, *memberId);
        bot.sendMessage(chatId, "Member deleted successfully");
    } else {
        storageApi.addMember(userId, state.storageId, *memberId);
        bot.sendMessage(chatId, "Member added successfully");
    }
    stateManager.put(StorageMemberView{state.storageId});
    renderMemberList(state.storageId, userId, chatId, bot, storageApi);
};

void cancelAddDeleteMember(
    MembersAdditionDeletion& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi) {
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;
    if (cq.data == "member_add_delete_cancel") {
        stateManager.put(StorageMemberView{state.storageId});
        renderMemberList(state.storageId, userId, chatId, bot, storageApi);
    }
};
} // namespace cookcookhnya::handlers::storageAddDeleteMembers
