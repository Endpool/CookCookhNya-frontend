#pragma once

#include "backSimulation.hpp"
#include "backend/id_types.hpp"
#include "types.hpp"
#include "utils.hpp"

#include <tgbot/Api.h>
#include <tgbot/types/CallbackQuery.h>
#include <tgbot/types/InlineKeyboardButton.h>
#include <tgbot/types/Message.h>

#include <format>
#include <iterator>
#include <memory>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace cookcookhnya {

using StorageId = api::StorageId;

namespace StorageRepositoryClass {

class Storage {
  public:
    StorageId storageId = 0;
    std::vector<std::string> content;
    std::string name;
    UserId ownerId = 0;
    std::vector<int> membersId;

    Storage(
        int storageId, std::string name, std::vector<std::string> content, int ownerId, std::vector<int> membersId) {
        this->storageId = storageId;
        this->name = name;
        std::copy(content.begin(), content.begin() + content.size(), this->content.begin());
        std::copy(membersId.begin(), membersId.begin() + membersId.size(), this->membersId.begin());
        this->ownerId = ownerId;
    }

    Storage() {
        this->storageId = 0;
        this->name = "Null";
        this->content = {"null"};
        this->ownerId = 0;
        this->membersId = {1};
    }

    bool addMember(StorageId storageId, UserId userId) {
        return true;
    }

    static Storage getNull() {
        return Storage{};
    }

    static Storage get(StorageId storageId) {
        return getNull();
    }

    std::vector<StorageId> getMembers(StorageId storageId) {
        std::vector<StorageId> members = {1, 2};
        return members;
    }
};
} // namespace StorageRepositoryClass

namespace render {

using namespace TgBot;
using InlineKeyboard = std::vector<std::vector<TgBot::InlineKeyboardButton::Ptr>>;
using MessageRef = const Message&;
using BotRef = const Api&;

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

inline void renderStorageList(UserId userId, ChatId chatId, BotRef bot) {
    // auto packs = StickerPackRepository::getUserPacks(userId); - get from id
    // char storageData[1000] = {""};
    auto currentStor = backendEx.getUserStorages(userId);
    InlineKeyboard keyboard(1 + ((currentStor.size() + 1) / 2)); // ceiling
    keyboard[0].reserve(2);
    keyboard[0].push_back(detail::makeCallbackButton("Add new storage", "StorageViewCreate"));
    keyboard[0].push_back(detail::makeCallbackButton("Delete existing storage", "StorageViewDelete"));
    for (uint32_t i = 0; i < currentStor.size(); i++) {
        if (i % 2 == 0)
            keyboard[1 + (i / 2)].reserve(2);

        // sprintf(storageData, "storage %d", currentStor[i].getId(userId)); // Create uniqie data for storage (one of
        // storages) button
        //  it also saves the id of storage
        keyboard[1 + (i / 2)].push_back(
            detail::makeCallbackButton(*currentStor[i].getName(), std::to_string(currentStor[i].getId(userId))));
    }

    bot.sendMessage(chatId, "Your storages:", nullptr, nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
}

inline void renderStorageView(StorageId storageId, UserId userId, ChatId chatId, BotRef bot) {
    // auto storage = StorageRepository::get(storageId);
    auto storage = StorageRepositoryClass::Storage::getNull(); // temporarily
    unsigned int buttonRows = 3;

    InlineKeyboard keyboard(buttonRows);
    keyboard[0].reserve(2);
    keyboard[0].push_back(detail::makeCallbackButton("Explore", "explore"));
    keyboard[0].push_back(detail::makeCallbackButton("Members", "members"));
    keyboard[0].push_back(detail::makeCallbackButton("Back", "back"));

    bot.sendMessage(chatId,
                    std::format("Storage \"{}\"", storage.name),
                    nullptr,
                    nullptr,
                    detail::makeKeyboardMarkup(std::move(keyboard)),
                    "MarkdownV2");
}

inline void renderMemberList(const StorageId& storageId, UserId userId, ChatId chatId, BotRef bot) {
    // auto storage = StorageRepository::get(storageId);
    auto storage = StorageRepositoryClass::Storage::getNull(); // temporarily
    bool isOwner = storage.ownerId == userId;
    unsigned int buttonRows = isOwner ? 2 : 1;

    InlineKeyboard keyboard(1);
    keyboard[0].push_back(detail::makeCallbackButton("Back", "back"));
    if (isOwner) {
        keyboard[0].push_back(detail::makeCallbackButton("Add/Delete member", "add_delete_member"));
    }

    std::string list;
    for (auto [i, id] : std::views::enumerate(storage.getMembers(storageId)))
        std::format_to(std::back_inserter(list), "{}. \'{}\'", i + 1, id);
    bot.sendMessage(chatId,
                    std::format("Here is the member list of \"{}\" storage.", list),
                    nullptr,
                    nullptr,
                    detail::makeKeyboardMarkup(std::move(keyboard)));
}

inline void renderMemberAdditionDeletionPrompt(const StorageId& storageId, ChatId chatId, BotRef bot) {
    // auto storage = StorageRepository::get(storageId);
    auto storage = StorageRepositoryClass::Storage::getNull(); // temporarily
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
    return;
}

inline void renderStorageCreate(ChatId chatId, BotRef bot) { // BackendProvider bkn
    // Here is connection to backend using chatId
    InlineKeyboard keyboard(1);
    keyboard[0].push_back(detail::makeCallbackButton("Cancel", "StorageCreateCancel"));
    bot.sendMessage(
        chatId, "Enter storage name to create", nullptr, nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
}

inline void renderStorageDelete(ChatId chatId, BotRef bot) { // BackendProvider bkn
    // Here is connection to backend using chatId
    InlineKeyboard keyboard(1);
    keyboard[0].push_back(detail::makeCallbackButton("Cancel", "StorageDeleteCancel"));
    bot.sendMessage(
        chatId, "Enter storage name to delete", nullptr, nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
}

} // namespace render

} // namespace cookcookhnya
