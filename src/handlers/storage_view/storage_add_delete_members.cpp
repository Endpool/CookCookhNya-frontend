#include "storage_add_delete_members.hpp"

#include "extern.hpp"
#include "handlers/common.hpp"
#include "render/common.hpp"
#include "render/storage_view/storage_members_render.hpp"
#include "tg_types.hpp"
#include "utils.hpp"

namespace cookcookhnya::handlers::storage_add_delete_members {

using namespace render::storage::member_list;

void addDeleteMember(
    MembersAdditionDeletion& state, MessageRef m, BotRef bot, SMRef stateManager, StorageApiRef storageApi) {
    auto chatId = m.chat->id;
    auto userId = m.from->id;
    auto memberId = utils::parseSafe<tg_types::UserId>(m.text);
    auto storage = storageApi.get(userId, state.storageId);

    if (!memberId) {
        auto messageId = cookcookhnya::message::getMessageId(userId);
        auto text = utils::utf8str(u8"Введен неправильный ID пользователя");
        InlineKeyboard keyboard(1);
        keyboard.bot.editMessageText(text, chatId, *messageId, "", "", nullptr);
        renderMemberAdditionDeletionPrompt(state.storageId, userId, chatId, bot, storageApi);
        return;
    }
    auto members = storageApi.getStorageMembers(userId, state.storageId);
    bool isMemberof = std::ranges::find(members, *memberId) != members.end();
    if (isMemberof) {
        storageApi.deleteMember(userId, state.storageId, *memberId);

    } else {
        storageApi.addMember(userId, state.storageId, *memberId);
        bot.sendMessage(chatId, "Member added successfully");
    }
    renderMemberList(state.storageId, userId, chatId, bot, storageApi);
    stateManager.put(StorageMemberView{state.storageId});
};

void cancelAddDeleteMember(
    MembersAdditionDeletion& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi) {
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;
    if (cq.data == "cancel_member_addition_deletion") {
        renderMemberList(state.storageId, userId, chatId, bot, storageApi);
        stateManager.put(StorageMemberView{state.storageId});
    }
};

} // namespace cookcookhnya::handlers::storage_add_delete_members
