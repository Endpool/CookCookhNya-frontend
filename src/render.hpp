#pragma once

#include "types.hpp"
#include "utils.hpp"

#include <tgbot/Api.h>
#include <tgbot/types/CallbackQuery.h>
#include <tgbot/types/InlineKeyboardButton.h>
#include <tgbot/types/Message.h>
#include <uuid.h>

#include <format>
#include <iterator>
#include <memory>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace render {

using namespace TgBot;

namespace detail {

inline std::shared_ptr<InlineKeyboardButton> makeCallbackButton(std::string_view text, std::string_view data) {
    InlineKeyboardButton button{};
    button.text = text;
    button.callbackData = data;
    return utils::make_shared(std::move(button));
}

inline std::shared_ptr<InlineKeyboardMarkup> makeKeyboardMarkup(InlineKeyboard&& keyboard) {
    auto markup = std::make_shared<InlineKeyboardMarkup>();
    markup->inlineKeyboard = std::move(keyboard);
    return markup;
}

} // namespace detail

inline void renderMemberAdditionDeletionPrompt(const StorageId& storageId, ChatId chatId, BotRef bot) {
    auto storage = StorageRepository::get(storageId);
    bool isOwner = storage.ownerId == userId;
    unsigned int buttonRows = isOwner ? 2 : 1;

    InlineKeyboard keyboard(1);
    keyboard[0].push_back(detail::makeCallbackButton("Back", "back"));
    if (isOwner){
        keyboard[0].push_back(detail::makeCallbackButton("Add/Delete member", "add_delete_member"));
    }

    std::string list;
    for (auto [i, id] : std::views::enumerate(PackSharingRepository::getMembers(storageId)))
        std::format_to(std::back_inserter(list), "{}. {}", i + 1, id);
    bot.sendMessage(chatId,
                    std::format("Here is the member list of \"{}\" storage."
                                "Send a Telegram ID to add/remove from the list.\n{}",
                                list),
                    nullptr,
                    nullptr,
                    detail::makeKeyboardMarkup(std::move(keyboard)));
}

inline void renderStorageView(StorageId storageId, UserId userId, ChatId chatId, BotRef bot) {
    auto storage = StorageRepository::get(packId);
    unsigned int buttonRows = 3;
    
    InlineKeyboard keyboard(buttonRows);
    keyboard[0].reserve(2);
    keyboard[0].push_back(detail::makeCallbackButton("Explore", "explore"));
    keyboard[0].push_back(detail::makeCallbackButton("Members", "members"));
    keyboard[0].push_back(detail::makeCallbackButton("Back", "back"));

    bot.sendMessage(chatId,
                    std::format("Storage \"{}\"",
                                pack.name,
                                uuids::to_string(packId),
                    nullptr,
                    nullptr,
                    detail::makeKeyboardMarkup(std::move(keyboard)),
                    "MarkdownV2");
} 


} // namespace render