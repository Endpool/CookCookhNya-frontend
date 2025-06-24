#include "storage_members_render.hpp"

#include "render/common.hpp"
#include "utils.hpp"

#include <format>
#include <iterator>
#include <ranges>
#include <utility>

namespace cookcookhnya::render::view_storage_members {

void renderMemberList(const StorageId& storageId, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi) {
    auto storage = storageApi.get(userId, storageId);
    bool isOwner = storage.ownerId == userId;
    unsigned int buttonRows = isOwner ? 2 : 1;

    InlineKeyboard keyboard(buttonRows);

    if (isOwner) {
        keyboard[0].push_back(detail::makeCallbackButton(utils::utf8str(u8"Редактировать"), "add_delete_member"));
        keyboard[1].push_back(detail::makeCallbackButton(utils::utf8str(u8"Назад"), "back_to_view_storage"));
    } else {
        keyboard[0].push_back(detail::makeCallbackButton(utils::utf8str(u8"Назад"), "back_to_view_storage"));
    }

    std::string list;
    for (auto [i, id] : std::views::enumerate(storageApi.getStorageMembers(userId, storageId)))
        std::format_to(std::back_inserter(list), "{}\\. {}\n", i + 1, id);
    bot.sendMessage(chatId,
                    utils::utf8str(u8"Участники хранилища:\n ") + list,
                    nullptr,
                    nullptr,
                    detail::makeKeyboardMarkup(std::move(keyboard)),
                    "MarkdownV2");
};

void renderMemberAdditionDeletionPrompt(
    const StorageId& storageId, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi) {
    auto storage = storageApi.get(userId, storageId);
    unsigned int buttonRows = 1;

    InlineKeyboard keyboard(buttonRows);
    keyboard[0].push_back(detail::makeCallbackButton(utils::utf8str(u8"Отмена"), "cancel_member_addition_deletion"));

    bot.sendMessage(chatId,
                    utils::utf8str(u8"Отправь Telegram ID для добавления/удаления из списка:\n "),
                    nullptr,
                    nullptr,
                    detail::makeKeyboardMarkup(std::move(keyboard)));
};

} // namespace cookcookhnya::render::view_storage_members
