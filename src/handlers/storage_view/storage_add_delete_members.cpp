#include "storage_add_delete_members.hpp"

#include "backend/models/storage.hpp"
#include "handlers/common.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "render/storage_view/storage_members_render.hpp"
#include "tg_types.hpp"
#include "utils.hpp"

namespace cookcookhnya::handlers::storage_add_delete_members {

using namespace render::storage::member_list;
using namespace render;

void addDeleteMember(
    MembersAdditionDeletion& state, MessageRef m, BotRef bot, SMRef stateManager, StorageApiRef storageApi) {
    auto chatId = m.chat->id;
    auto userId = m.from->id;
    auto memberId = utils::parseSafe<tg_types::UserId>(m.text);
    auto storage = storageApi.get(userId, state.storageId);

    InlineKeyboard keyboard(1);
    keyboard[0].push_back(detail::makeCallbackButton(utils::utf8str(u8"Назад"), "cancel_member_addition_deletion"));

    if (!memberId) {
        auto text = utils::utf8str(u8"Введен неправильный ID пользователя");
        bot.sendMessage(chatId, text);
        renderMemberList(state.storageId, userId, chatId, bot, storageApi);
        return;
    }
    auto members = storageApi.getStorageMembers(userId, state.storageId);
    bool isMemberOf =
        std::ranges::find(members, memberId, &api::models::storage::StorageMemberDetails::userId) != members.end();
    if (isMemberOf) {
        storageApi.deleteMember(userId, state.storageId, *memberId);
        auto text = utils::utf8str(u8"Участник успешно удален");
        bot.sendMessage(chatId, text);
    } else {
        storageApi.addMember(userId, state.storageId, *memberId);
        bot.sendMessage(chatId, "Участник успешно добавлен");
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
