#pragma once

#include "backend/id_types.hpp"
#include "backend/models/storage.hpp"
#include "types.hpp"
#include "utils.hpp"

#include <tgbot/Api.h>
#include <tgbot/types/CallbackQuery.h>
#include <tgbot/types/InlineKeyboardButton.h>
#include <tgbot/types/Message.h>
#include <backend/api/storages.hpp>

#include <cstdio>
#include <format>
#include <iterator>
#include <memory>
#include <ranges>
#include <string>
#include <string_view>
#include <tgbot/types/User.h>
#include <utility>
#include <vector>

namespace cookcookhnya {

using StorageId = api::StorageId;

namespace render {
  
using namespace TgBot;
using namespace cookcookhnya::api;

using InlineKeyboard = std::vector<std::vector<TgBot::InlineKeyboardButton::Ptr>>;
using MessageRef = const Message&;
using BotRef = const Api&;
using BackendApiRef = const StoragesApi&;

namespace detail {
inline std::shared_ptr<TgBot::InlineKeyboardButton> makeCallbackButton(std::string_view text, std::string_view data) {
    TgBot::InlineKeyboardButton button{};
    button.text = text;
    button.callbackData = data;
    return utils::make_shared(std::move(button));
}

inline std::shared_ptr<TgBot::InlineKeyboardMarkup> makeKeyboardMarkup(InlineKeyboard&& keyboard) {
    auto markup = std::make_shared<TgBot::InlineKeyboardMarkup>();
    markup->inlineKeyboard = std::move(keyboard);
    return markup;
}
} // namespace detail

inline void renderStorageList(UserId userId, ChatId chatId, BotRef bot, BackendApiRef api) {

    auto currentStor = api.getStoragesList(userId); // Take storages of user from backend

    InlineKeyboard keyboard(1 + ((currentStor.size() + 1) / 2)); // ceiling
    if (currentStor.size()!=0){
        keyboard[0].reserve(2);
        keyboard[0].push_back(detail::makeCallbackButton("Add new storage", "StorageViewCreate"));
        keyboard[0].push_back(detail::makeCallbackButton("Delete existing storage", "StorageViewDelete"));
    } else {
        keyboard[0].reserve(1);
        keyboard[0].push_back(detail::makeCallbackButton("Add new storage", "StorageViewCreate"));
    }

    for (uint32_t i = 0; i < currentStor.size(); i++) {
        if (i % 2 == 0)
            keyboard[1 + (i / 2)].reserve(2);
        keyboard[1 + (i / 2)].push_back(
            detail::makeCallbackButton(currentStor[i].name, std::to_string(currentStor[i].id)));
    }

    bot.sendMessage(chatId, "Your storages:", nullptr, nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
}

inline void renderStorageView(StorageId storageId, UserId userId, ChatId chatId, BotRef bot, BackendApiRef api) {
    auto storage = api.get(userId, storageId);
    unsigned int buttonRows = 3;
    InlineKeyboard keyboard(1);
    keyboard[0].reserve(3);
    keyboard[0].push_back(detail::makeCallbackButton("Explore", "explore"));
    keyboard[0].push_back(detail::makeCallbackButton("Members", "members"));
    keyboard[0].push_back(detail::makeCallbackButton("Back", "back"));

    bot.sendMessage(chatId,
                    storage.name,
                    nullptr,
                    nullptr,
                    detail::makeKeyboardMarkup(std::move(keyboard)));
}

inline void renderMemberList(const StorageId& storageId, UserId userId, ChatId chatId, BotRef bot, BackendApiRef api) {
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
}

inline void renderMemberAdditionDeletionPrompt(const StorageId& storageId, UserId userId, ChatId chatId, BotRef bot, BackendApiRef api) {
    auto storage = api.get(userId, storageId);
    unsigned int buttonRows = 1;

    InlineKeyboard keyboard(1);
    keyboard[0].push_back(detail::makeCallbackButton("Cancel", "member_add_delete_cancel"));

    bot.sendMessage(chatId,
                    std::format("Send a Telegram ID to add/remove from the list."),
                    nullptr,
                    nullptr,
                    detail::makeKeyboardMarkup(std::move(keyboard)));
}

inline void renderIngredientsList(StorageId storageId, UserId userId, ChatId chatId, BotRef bot) {
    // auto ad = backendEx.getUserStorages(userId)[0];
    // std::string resultStr ="";
    // for (int i=0;i<ad.getContent(123).size();i++){
    //     resultStr += std::format("{} \n", ad.getContent(123)[i]);
    // }
    //     bot.sendMessage(chatId,
    //                 resultStr);

    return;
}

inline void renderStorageCreate(ChatId chatId, BotRef bot, UserId userId) { // BackendProvider bkn
    InlineKeyboard keyboard(1);
    keyboard[0].push_back(detail::makeCallbackButton("Cancel", "StorageCreateCancel"));
    bot.sendMessage(
        chatId, "Enter new storage name", nullptr, nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
}

inline void renderStorageDelete(ChatId chatId, BotRef bot, UserId userId,  BackendApiRef api) {
    auto currentStor = api.getStoragesList(userId); // Take all storages from backend so user could choose which to delete
    InlineKeyboard keyboard(1 + ((currentStor.size() + 1) / 2)); // ceiling
    keyboard[0].reserve(1);
    keyboard[0].push_back(detail::makeCallbackButton("Cancel", "StorageDeleteCancel"));
    for (uint32_t i = 0; i < currentStor.size(); i++) {
        if (i % 2 == 0)
            keyboard[1 + (i / 2)].reserve(2);
        keyboard[1 + (i / 2)].push_back(
            detail::makeCallbackButton(currentStor[i].name, std::to_string(currentStor[i].id)));
    }
    bot.sendMessage(
        chatId, "Choose storage to delete", nullptr, nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
}


} // namespace render

} // namespace cookcookhnya
