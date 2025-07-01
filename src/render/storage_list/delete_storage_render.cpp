#include "delete_storage_render.hpp"

#include "render/common.hpp"
#include "utils.hpp"

#include <cstddef>
#include <string>
#include <utility>

namespace cookcookhnya::render::delete_storage {

void renderStorageDelete(ChatId chatId, BotRef bot, UserId userId, StorageApiRef storageApi) {
    auto currentStorages =
        storageApi.getStoragesList(userId); // Take all storages from backend so user could choose which to delete
    InlineKeyboard keyboard(1 + ((currentStorages.size() + 1) / 2)); // ceiling
    keyboard[0].push_back(detail::makeCallbackButton(u8"Отмена", "cancel_storage_deletion"));
    for (std::size_t i = 0; i < currentStorages.size(); i++) {
        if (i % 2 == 0)
            keyboard[1 + (i / 2)].reserve(2);
        keyboard[1 + (i / 2)].push_back(
            detail::makeCallbackButton(currentStorages[i].name, std::to_string(currentStorages[i].id)));
    }
    bot.sendMessage(chatId,
                    utils::utf8str(u8"Что вы хотите удалить"),
                    nullptr,
                    nullptr,
                    detail::makeKeyboardMarkup(std::move(keyboard)));
};

} // namespace cookcookhnya::render::delete_storage
