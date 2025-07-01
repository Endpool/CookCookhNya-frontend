#include "storage_members_render.hpp"

#include "backend/id_types.hpp"
#include "render/common.hpp"
#include "utils.hpp"

#include <cstddef>
#include <format>
#include <iterator>
#include <ranges>
#include <string>
#include <utility>

namespace cookcookhnya::render::storage::member_list {

void renderMemberList(api::StorageId storageId, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi) {
    auto storage = storageApi.get(userId, storageId);
    const bool isOwner = storage.ownerId == userId;
    const std::size_t buttonRows = isOwner ? 2 : 1;

    InlineKeyboard keyboard(buttonRows);

    if (isOwner) {
        keyboard[0].push_back(detail::makeCallbackButton(u8"Редактировать", "add_delete_member"));
        keyboard[1].push_back(detail::makeCallbackButton(u8"Назад", "back_to_view_storage"));
    } else {
        keyboard[0].push_back(detail::makeCallbackButton(u8"Назад", "back_to_view_storage"));
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
    api::StorageId storageId, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi) {
    auto storage = storageApi.get(userId, storageId);
    const std::size_t buttonRows = 1;

    InlineKeyboard keyboard(buttonRows);
    keyboard[0].push_back(detail::makeCallbackButton(u8"Отмена", "cancel_member_addition_deletion"));

    bot.sendMessage(chatId,
                    utils::utf8str(u8"Отправь Telegram ID для добавления/удаления из списка:\n "),
                    nullptr,
                    nullptr,
                    detail::makeKeyboardMarkup(std::move(keyboard)));
};

} // namespace cookcookhnya::render::storage::member_list
