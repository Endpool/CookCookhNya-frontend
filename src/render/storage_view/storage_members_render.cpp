#include "storage_members_render.hpp"

#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils.hpp"
#include <format>
#include <iterator>
#include <ranges>
#include <string>
#include <utility>

namespace cookcookhnya::render::storage::member_list {

void renderMemberList(
    bool toBeEdited, const StorageId& storageId, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi) {
    auto storage = storageApi.get(userId, storageId);
    bool isOwner = storage.ownerId == userId;
    unsigned int buttonRows = isOwner ? 2 : 1;

    InlineKeyboard keyboard(buttonRows);

    if (isOwner) {
        keyboard[0].push_back(detail::makeCallbackButton(utils::utf8str(u8"🔐 Добавить"), "add_member"));
        if (storageApi.getStorageMembers(userId, storageId).size() > 1) {
            keyboard[0].push_back(detail::makeCallbackButton(utils::utf8str(u8"🔒 Удалить"), "delete_member"));
        }
        keyboard[1].push_back(detail::makeCallbackButton(utils::utf8str(u8"↩️Назад"), "back_to_view_storage"));
    } else {
        keyboard[0].push_back(detail::makeCallbackButton(utils::utf8str(u8"↩️Назад"), "back_to_view_storage"));
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
        bot.editMessageText(text, chatId, *messageId, "", "", nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
    } else {
        auto messageId =
            bot.sendMessage(chatId, text, nullptr, nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
        message::addMessageId(userId, messageId->messageId);
    }
};

void renderMemberAdditionPrompt(
    const StorageId& storageId, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi) {
    auto storage = storageApi.get(userId, storageId);
    unsigned int buttonRows = 1;

    InlineKeyboard keyboard(buttonRows);
    keyboard[0].push_back(detail::makeCallbackButton(utils::utf8str(u8"🚫 Отмена"), "cancel_member_addition"));
    auto text = utils::utf8str(u8"📩 Перешлите сообщение пользователя, чтобы добавить его в хранилище\n");
    auto messageId = message::getMessageId(userId);
    bot.editMessageText(text, chatId, *messageId, "", "", nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
};

void renderMemberDeletionPrompt(
    const StorageId& storageId, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi) {
    auto storage = storageApi.get(userId, storageId);

    auto members = storageApi.getStorageMembers(userId, storageId);
    std::cerr << userId << storage.ownerId << '\n';
    unsigned int buttonRows = members.size();
    InlineKeyboard keyboard(buttonRows);
    keyboard[0].push_back(detail::makeCallbackButton(utils::utf8str(u8"🚫 Отмена"), "cancel_member_deletion"));
    size_t k = 1;
    for (auto& member : members) {
        if (member.userId != storage.ownerId) {
            keyboard[k++].push_back(detail::makeCallbackButton(utils::utf8str(u8"👤 ") + member.fullName,
                                                               "mem_" + std::to_string(member.userId)));
        }
    }

    auto text = utils::utf8str(u8"🚷 Выберите участника для удаления\n");
    auto messageId = message::getMessageId(userId);
    bot.editMessageText(text, chatId, *messageId, "", "", nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
};

} // namespace cookcookhnya::render::storage::member_list
