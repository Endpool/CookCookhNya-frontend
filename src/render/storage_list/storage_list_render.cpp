#include "storage_list_render.hpp"
#include "render/types_for_render.hpp"

namespace cookcookhnya::render::storageList {
using namespace cookcookhnya::render;
void renderStorageList(UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi) {

    auto currentStor = storageApi.getStoragesList(userId); // Take storages of user from backend

    InlineKeyboard keyboard(1 + ((currentStor.size() + 1) / 2)); // ceiling
    if (currentStor.size() != 0) {
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
} // namespace cookcookhnya::render::storageList
