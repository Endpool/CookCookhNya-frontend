#include "delete_storage_render.hpp"

#include "render/common.hpp"

namespace cookcookhnya::render::deleteStorage {

void renderStorageDelete(ChatId chatId, BotRef bot, UserId userId, StorageApiRef storageApi) {
    auto currentStor =
        storageApi.getStoragesList(userId); // Take all storages from backend so user could choose which to delete
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
};

} // namespace cookcookhnya::render::deleteStorage
