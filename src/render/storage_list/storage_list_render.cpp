#include "storage_list_render.hpp"

#include "render/common.hpp"
#include "utils.hpp"

#include <cstddef>
#include <string>
#include <utility>

namespace cookcookhnya::render::storage_list {

void renderStorageList(UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi) {
    auto currentStorages = storageApi.getStoragesList(userId); // Take storages of user from backend

    const std::size_t buttonRows = currentStorages.empty() ? 1 : ((currentStorages.size() + 1) / 2) + 2; // ceiling
    InlineKeyboard keyboard(buttonRows);


    unsigned long buttonRows = 0;
    if (!currentStor.empty()) {
        buttonRows = (currentStor.size() + 1) / 2 + 2;
    } else {
        buttonRows = 1;
    }
    InlineKeyboard keyboard(buttonRows);
    if (!currentStor.empty()) {

        keyboard[0].reserve(2);
        keyboard[0].push_back(detail::makeCallbackButton(u8"➕", "storage_list_creation"));
        keyboard[0].push_back(detail::makeCallbackButton(u8"➖", "storage_list_deletion"));
    } else {
        keyboard[0].push_back(detail::makeCallbackButton(u8"➕", "storage_list_creation"));
    }


    for (size_t i = 0; i < currentStor.size(); i++) {

        if (i % 2 == 0)
            keyboard[1 + (i / 2)].reserve(2);
        keyboard[1 + (i / 2)].push_back(
            detail::makeCallbackButton(currentStorages[i].name, std::to_string(currentStorages[i].id)));
    }

    if (!currentStorages.empty()) {
        keyboard[keyboard.size() - 1].push_back(
            detail::makeCallbackButton(u8"Хочу кушать", "storage_list_what_to_cook"));
    }

    bot.sendMessage(
        chatId, utils::utf8str(u8"Ваши хранилища:"), nullptr, nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
}

} // namespace cookcookhnya::render::storage_list
