#include "add.hpp"

#include "backend/id_types.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils.hpp"

#include <tgbot/types/InlineKeyboardButton.h>

#include <memory>
#include <string>
#include <utility>

namespace cookcookhnya::render::storage::add_member {

void renderMemberAdditionPrompt(
    const api::StorageId& storageId, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi) {
    auto storage = storageApi.get(userId, storageId);

    const int buttonRows = 2;
    InlineKeyboard keyboard(buttonRows);

    auto inviteButton = std::make_shared<TgBot::InlineKeyboardButton>();
    inviteButton->text = utils::utf8str(u8"📤 Поделиться");
    auto hash = storageApi.inviteMember(userId, storageId);
    const auto* telegramBotAlias = "stage_stand_bot";
    auto inviteText = "Нажми на ссылку, чтобы стать участником хранилища 🍱**" + storage.name +
                      "** в CookCookhNya!\nhttps://t.me/" + telegramBotAlias + "?start=" + hash;
    inviteButton->url = "https://t.me/share/url?url=" + inviteText;
    inviteButton->callbackData = "user_sended_link";

    keyboard[0].push_back(std::move(inviteButton));
    keyboard[1].push_back(detail::makeCallbackButton(u8"↩️ Назад", "cancel_member_addition"));
    auto text = utils::utf8str(
        u8"📩 Нажмите кнопку ниже или перешлите сообщение пользователя, чтобы добавить его в хранилище\n");
    auto messageId = message::getMessageId(userId);
    if (messageId) {
        bot.editMessageText(text, chatId, *messageId, "", "", nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
    }
};

} // namespace cookcookhnya::render::storage::add_member
