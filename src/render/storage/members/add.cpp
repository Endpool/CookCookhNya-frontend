#include "add.hpp"

#include "backend/api/storages.hpp"
#include "backend/id_types.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils/u8format.hpp"
#include "utils/utils.hpp"

#include <boost/url/url.hpp>
#include <tgbot/types/InlineKeyboardButton.h>

#include <cstddef>
#include <memory>
#include <string>
#include <utility>

namespace cookcookhnya::render::storage::members {

void renderStorageMemberAddition(
    const api::StorageId& storageId, UserId userId, ChatId chatId, BotRef bot, api::StorageApiRef storageApi) {
    auto storage = storageApi.get(userId, storageId);

    const std::size_t buttonRows = 2;
    InlineKeyboard keyboard(buttonRows);
    keyboard[0].push_back(makeCallbackButton(u8"🔗 Создать ссылку", "create_link"));
    keyboard[1].push_back(makeCallbackButton(u8"↩️ Назад", "back"));
    auto text =
        utils::utf8str(u8"📩 Создайте ссылку или перешлите сообщение пользователя, чтобы добавить его в хранилище.\n");
    auto messageId = message::getMessageId(userId);
    if (messageId)
        bot.editMessageText(text, chatId, *messageId, makeKeyboardMarkup(std::move(keyboard)));
};

void renderShareLinkMemberAddition(
    const api::StorageId& storageId, UserId userId, ChatId chatId, BotRef bot, api::StorageApiRef storageApi) {
    auto storage = storageApi.get(userId, storageId);

    const std::size_t buttonRows = 2;
    InlineKeyboard keyboard{buttonRows};

    auto inviteButton = std::make_shared<TgBot::InlineKeyboardButton>();
    inviteButton->text = utils::utf8str(u8"📤 Поделиться");
    const std::string botAlias = bot.getUnderlying().getMe()->username;
    const api::InvitationId hash = storageApi.inviteMember(userId, storageId);
    const std::string storageUrl = std::format("https://t.me/{}?start=invite_{}", botAlias, hash);
    const std::string shareText = utils::u8format(
        "{} **{}** {}", u8"Нажми на ссылку, чтобы стать участником хранилища 🍱", storage.name, "в CookCookhNya!");

    boost::urls::url url{"https://t.me/share/url"};
    url.params().append({"url", storageUrl});
    url.params().append({"text", shareText});
    inviteButton->url = url.buffer();

    keyboard[0].push_back(std::move(inviteButton));
    keyboard[1].push_back(makeCallbackButton(u8"↩️ Назад", "back"));
    auto text = utils::utf8str(
        u8"📩 Нажмите кнопку ниже или перешлите сообщение пользователя, чтобы добавить его в хранилище.\n");
    auto messageId = message::getMessageId(userId);
    if (messageId)
        bot.editMessageText(text, chatId, *messageId, makeKeyboardMarkup(std::move(keyboard)));
};

} // namespace cookcookhnya::render::storage::members
