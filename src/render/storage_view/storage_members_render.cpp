#include "storage_members_render.hpp"

#include "backend/id_types.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils.hpp"

#include <tgbot/types/InlineKeyboardButton.h>

#include <cstddef>
#include <format>
#include <iterator>
#include <memory>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

namespace cookcookhnya::render::storage::member_list {

void renderMemberList(bool toBeEdited,
                      const api::StorageId& storageId,
                      UserId userId,
                      ChatId chatId,
                      BotRef bot,
                      StorageApiRef storageApi) {
    auto storage = storageApi.get(userId, storageId);
    const bool isOwner = storage.ownerId == userId;
    const int buttonRows = isOwner ? 2 : 1;

    InlineKeyboard keyboard(buttonRows);

    if (isOwner) {
        keyboard[0].push_back(detail::makeCallbackButton(u8"🔐 Добавить", "add_member"));
        if (storageApi.getStorageMembers(userId, storageId).size() > 1) {
            keyboard[0].push_back(detail::makeCallbackButton(u8"🔒 Удалить", "delete_member"));
        }
        keyboard[1].push_back(detail::makeCallbackButton(u8"↩️Назад", "back_to_view_storage"));
    } else {
        keyboard[0].push_back(detail::makeCallbackButton(u8"↩️Назад", "back_to_view_storage"));
    }

    std::string list;
    std::vector<std::string> memberNames;
    for (const auto& memberDetails : storageApi.getStorageMembers(userId, storageId)) {
        memberNames.push_back(memberDetails.fullName);
    }
    for (auto [i, name] : std::views::enumerate(memberNames))
        std::format_to(std::back_inserter(list), "  {}. {}\n", i + 1, name);
    auto text = utils::utf8str(u8"👥 Список участников\n") + list;
    if (toBeEdited) {
        auto messageId = message::getMessageId(userId);
        if (messageId) {
            bot.editMessageText(
                text, chatId, *messageId, "", "", nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
        }
    } else {
        auto messageId =
            bot.sendMessage(chatId, text, nullptr, nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
        message::addMessageId(userId, messageId->messageId);
    }
};

void renderMemberAdditionPrompt(
    const api::StorageId& storageId, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi) {
    auto storage = storageApi.get(userId, storageId);

    const int buttonRows = 2;
    InlineKeyboard keyboard(buttonRows);

    auto inviteButton = std::make_shared<TgBot::InlineKeyboardButton>();
    inviteButton->text = utils::utf8str(u8"📤 Поделиться");
    auto hash = "hashFromBack"; // NOLINT
    // TODO: get here hash from back
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

void renderMemberDeletionPrompt(
    const api::StorageId& storageId, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi) {
    auto storage = storageApi.get(userId, storageId);

    auto members = storageApi.getStorageMembers(userId, storageId);
    const unsigned int buttonRows = members.size();
    InlineKeyboard keyboard(buttonRows);
    keyboard[0].push_back(detail::makeCallbackButton(u8"🚫 Отмена", "cancel_member_deletion"));
    size_t k = 1;
    for (auto& member : members) {
        if (member.userId != storage.ownerId) {
            keyboard[k++].push_back(detail::makeCallbackButton(
                std::format("{} {}", utils::utf8str(u8"👤"), member.fullName), "mem_" + std::to_string(member.userId)));
        }
    }

    auto text = utils::utf8str(u8"🚷 Выберите участника для удаления\n");
    auto messageId = message::getMessageId(userId);
    if (messageId) {
        bot.editMessageText(text, chatId, *messageId, "", "", nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
    }
};

} // namespace cookcookhnya::render::storage::member_list
