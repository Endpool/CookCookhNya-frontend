#include "add.hpp"

#include "handlers/common.hpp"
#include "message_tracker.hpp"
#include "render/storage/members/add.hpp"
#include "render/storage/members/view.hpp"
#include "utils.hpp"

#include <tgbot/types/MessageOriginHiddenUser.h>
#include <tgbot/types/MessageOriginUser.h>

#include <memory>
#include <stdexcept>

namespace cookcookhnya::handlers::storage::members {

using namespace render::storage::members;

void handleStorageMemberAdditionMsg(
    StorageMemberAddition& state, MessageRef m, BotRef bot, SMRef stateManager, StorageApiRef storageApi) {
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

void handleStorageMemberAdditionCQ(
    StorageMemberAddition& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi) {
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;
    if (cq.data == "back") {
        renderMemberList(true, state.storageId, userId, chatId, bot, storageApi);
        stateManager.put(StorageMemberView{state.storageId});
    }
    if (cq.data == "create_link") {
        renderShareLinkMemberAddition(state.storageId, userId, chatId, bot, storageApi);
    }
};

} // namespace cookcookhnya::handlers::storage::members
