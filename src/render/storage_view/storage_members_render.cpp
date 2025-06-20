#include "storage_members_render.hpp"

namespace cookcookhnya::render::viewStorageMembers{
using namespace cookcookhnya::render;

void renderMemberList(const StorageId& storageId, UserId userId, ChatId chatId, BotRef bot, BackendApiRef api) {
    auto storage = api.get(userId, storageId);
    bool isOwner = storage.ownerId == userId;
    unsigned int buttonRows = isOwner ? 2 : 1;

    InlineKeyboard keyboard(1);
    keyboard[0].push_back(detail::makeCallbackButton("Back", "back"));
    if (isOwner) {
        keyboard[0].push_back(detail::makeCallbackButton("Add/Delete member", "add_delete_member"));
    }

    std::string list;
    for (auto [i, id] : std::views::enumerate(api.getStorageMembers(userId, storageId)))
        std::format_to(std::back_inserter(list), "{}. \'{}\'", i + 1, id);
    bot.sendMessage(chatId,
                    std::format("Here is the member list of \"{}\" storage.", list),
                    nullptr,
                    nullptr,
                    detail::makeKeyboardMarkup(std::move(keyboard)));
};

void renderMemberAdditionDeletionPrompt(const StorageId& storageId, UserId userId, ChatId chatId, BotRef bot, BackendApiRef api) {
    auto storage = api.get(userId, storageId);
    unsigned int buttonRows = 1;

    InlineKeyboard keyboard(1);
    keyboard[0].push_back(detail::makeCallbackButton("Cancel", "member_add_delete_cancel"));

    bot.sendMessage(chatId,
                    std::format("Send a Telegram ID to add/remove from the list."),
                    nullptr,
                    nullptr,
                    detail::makeKeyboardMarkup(std::move(keyboard)));
};

}  // namespace cookcookhnya::render::viewStorageMembers
