#include "add.hpp"

#include "handlers/common.hpp"
#include "message_tracker.hpp"
#include "render/storage_view//members/add.hpp"
#include "render/storage_view/members/view.hpp"
#include "utils.hpp"

#include <memory>
#include <stdexcept>
#include <tgbot/types/MessageOriginHiddenUser.h>
#include <tgbot/types/MessageOriginUser.h>

namespace cookcookhnya::handlers::storage_add_member {

using namespace render::storage::member_list;
using namespace render;
using namespace render::storage::add_member;

void addMember(MemberAddition& state, MessageRef m, BotRef bot, SMRef stateManager, StorageApiRef storageApi) {
    auto chatId = m.chat->id;
    auto userId = m.from->id;

    auto originUser = std::dynamic_pointer_cast<TgBot::MessageOriginUser>(m.forwardOrigin);
    if (originUser != nullptr) {
        auto memberId = originUser->senderUser->id;
        try {
            storageApi.addMember(userId, state.storageId, memberId);
        } catch (std::runtime_error&) {
            auto text = utils::utf8str(u8"❌ Не удалось добавить: пользователь не зарегестрирован в CookCookhNya");
            bot.sendMessage(chatId, text);
        }
    } else {
        if (std::dynamic_pointer_cast<TgBot::MessageOriginHiddenUser>(m.forwardOrigin) != nullptr) {
            auto text = utils::utf8str(u8"❌ Не удалось добавить: пользователь скрыл аккаунт");
            bot.sendMessage(chatId, text);
        } else {
            auto text = utils::utf8str(u8"❌ Не удалось добавить: убедитесь, что переслали сообщение пользователя");
            bot.sendMessage(chatId, text);
        }
    }
    auto text = utils::utf8str(
        u8"📩 Нажмите кнопку ниже или перешлите сообщение пользователя, чтобы добавить его в хранилище\n");
    auto messageId = message::getMessageId(userId);
    if (messageId) {
        bot.editMessageText(text, chatId, *messageId);
    }
    renderMemberList(false, state.storageId, userId, chatId, bot, storageApi);
    stateManager.put(StorageMemberView{state.storageId});
};

void cancelMemberAddition(
    MemberAddition& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi) {
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;
    if (cq.data == "back") {
        renderMemberList(true, state.storageId, userId, chatId, bot, storageApi);
        stateManager.put(StorageMemberView{state.storageId});
    }
    if (cq.data == "create_link") {
        addShareLinkButton(state.storageId, userId, chatId, bot, storageApi);
    }
};

} // namespace cookcookhnya::handlers::storage_add_member
