#include "utils.hpp"
#include "types.hpp"
#include <tgbot/types/CallbackQuery.h>
#include <tgbot/types/InlineKeyboardButton.h>
#include "backSimulation.hpp"


#include <format>
#include <iterator>
#include <memory>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

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
}




inline void renderStorageView(UserId userId, ChatId chatId, BotRef bot) {
    //auto packs = StickerPackRepository::getUserPacks(userId); - get from id
    char storageData[1000];
    auto currentStor = backendEx.getUserStorages(userId);
    InlineKeyboard keyboard(1 + ((currentStor.size() + 1) / 2)); // ceiling
    keyboard[0].reserve(2);
    keyboard[0].push_back(::detail::makeCallbackButton("Add new storage", "StorageViewCreate"));
    keyboard[0].push_back(::detail::makeCallbackButton("Delete existing storage", "StorageViewDelete"));
    for (uint32_t i = 0; i<currentStor.size(); i++){
        if (i % 2 == 0)
            keyboard[1 + (i / 2)].reserve(2);
        
        sprintf(storageData, "storage %d", currentStor[i].getId()); // Create uniqie data for storage (one of storages) button
        keyboard[1 + (i / 2)].push_back(::detail::makeCallbackButton(currentStor[i].getName(), storageData));
    }


    bot.sendMessage(chatId, "Your storages:", nullptr, nullptr, ::detail::makeKeyboardMarkup(std::move(keyboard)));
}

inline void renderStorageCreate (ChatId chatId, BotRef bot){ //BackendProvider bkn
    // Here is connection to backend using chatId
    InlineKeyboard keyboard(1);
    keyboard[0].push_back(::detail::makeCallbackButton("Cancel", "StorageCreateCancel"));
    bot.sendMessage(chatId, "Enter storage name to create", nullptr, nullptr, ::detail::makeKeyboardMarkup(std::move(keyboard)));
}

inline void renderStorageDelete (ChatId chatId, BotRef bot){ //BackendProvider bkn
    // Here is connection to backend using chatId
    InlineKeyboard keyboard(1);
    keyboard[0].push_back(::detail::makeCallbackButton("Cancel", "StorageDeleteCancel"));
    bot.sendMessage(chatId, "Enter storage name to delete", nullptr, nullptr, ::detail::makeKeyboardMarkup(std::move(keyboard)));
}

