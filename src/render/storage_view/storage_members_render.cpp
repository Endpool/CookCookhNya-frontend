#include "storage_members_render.hpp"

#include "render/common.hpp"

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
        keyboard[0].push_back(detail::makeCallbackButton("Add/Delete member", "add_delete_member"));
        keyboard[1].push_back(detail::makeCallbackButton("Back", "back_to_view_storage"));
    } else {
        keyboard[0].push_back(detail::makeCallbackButton("Back", "back_to_view_storage"));
    }

    std::string list;
    for (auto [i, id] : std::ranges::views::enumerate(storageApi.getStorageMembers(userId, storageId)))
        std::format_to(std::back_inserter(list), "{}. \'{}\'", i + 1, id);
    bot.sendMessage(chatId,
                    std::format("Here is the member list of **{}** storage.", storage.name),
                    nullptr,
                    nullptr,
                    detail::makeKeyboardMarkup(std::move(keyboard)));
};

void renderMemberAdditionDeletionPrompt(
    const StorageId& storageId, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi) {
    auto storage = storageApi.get(userId, storageId);
    unsigned int buttonRows = 1;

    InlineKeyboard keyboard(buttonRows);
    keyboard[0].push_back(detail::makeCallbackButton("Cancel", "cancel_member_addition_deletion"));

    bot.sendMessage(chatId,
                    std::format("Send a Telegram ID to add/remove from the list."),
                    nullptr,
                    nullptr,
                    detail::makeKeyboardMarkup(std::move(keyboard)));
};

} // namespace cookcookhnya::render::view_storage_members
