#include "storage_list_render.hpp"

#include "extern.hpp"
#include "render/common.hpp"
#include "utils.hpp"

namespace cookcookhnya::render::storage_list {

using namespace tg_types;

void renderStorageList(UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi) {

    auto currentStor = storageApi.getStoragesList(userId); // Take storages of user from backend

    unsigned long buttonRows = 0;
    if (!currentStor.empty()) {
        buttonRows = (currentStor.size() + 1) / 2 + 2;
    } else {
        buttonRows = 1;
    }
    InlineKeyboard keyboard(buttonRows); // ceiling
    if (!currentStor.empty()) {
        keyboard[0].reserve(2);
        keyboard[0].push_back(detail::makeCallbackButton(utils::utf8str(u8"➕"), "storage_list_creation"));
        keyboard[0].push_back(detail::makeCallbackButton(utils::utf8str(u8"➖"), "storage_list_deletion"));
    } else {
        keyboard[0].reserve(1);
        keyboard[0].push_back(detail::makeCallbackButton(utils::utf8str(u8"➕"), "storage_list_creation"));
    }

    for (uint32_t i = 0; i < currentStor.size(); i++) {
        if (i % 2 == 0)
            keyboard[1 + (i / 2)].reserve(2);
        keyboard[1 + (i / 2)].push_back(
            detail::makeCallbackButton(currentStor[i].name, std::to_string(currentStor[i].id)));
    }
    if (!currentStor.empty()) {
        keyboard[((currentStor.size() + 1) / 2) + 1].push_back(
            detail::makeCallbackButton(utils::utf8str(u8"Хочу кушать"), "storage_list_what_to_cook"));
    }

    auto message = bot.sendMessage(
        chatId, utils::utf8str(u8"Ваши хранилища:"), nullptr, nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
    cookcookhnya::message::addMessageId(userId, message->messageId);
}

} // namespace cookcookhnya::render::storage_list
