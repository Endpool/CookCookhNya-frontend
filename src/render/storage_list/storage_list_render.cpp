#include "storage_list_render.hpp"

#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils.hpp"

#include <cstddef>
#include <string>
#include <utility>

namespace cookcookhnya::render::storage_list {

using namespace tg_types;

void renderStorageList(bool toBeEdited, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi) {

    auto currentStorages = storageApi.getStoragesList(userId); // Take storages of user from backend

    const std::size_t buttonRows =
        1 + (currentStorages.empty() ? 1 : ((currentStorages.size() + 1) / 2) + 2); // ceiling
    InlineKeyboard keyboard(buttonRows);

    if (!currentStorages.empty()) {
        keyboard[0].reserve(2);
        keyboard[0].push_back(detail::makeCallbackButton(utils::utf8str(u8"🆕 Создать"), "storage_list_creation"));
        keyboard[0].push_back(detail::makeCallbackButton(utils::utf8str(u8"🚮 Удалить"), "storage_list_deletion"));
    } else {
        keyboard[0].reserve(1);
        keyboard[0].push_back(detail::makeCallbackButton(utils::utf8str(u8"🆕 Создать"), "storage_list_creation"));
    }

    for (uint32_t i = 0; i < currentStor.size(); i++) {
        if (i % 2 == 0)
            keyboard[1 + (i / 2)].reserve(2);
        keyboard[1 + (i / 2)].push_back(detail::makeCallbackButton(utils::utf8str(u8"🍱 ") + currentStor[i].name,
                                                                   std::to_string(currentStor[i].id)));
    }
    
    if (!currentStorages.empty()) {
        keyboard[keyboard.size() - 2].push_back(detail::makeCallbackButton(u8"Список покупок", "shopping_list"));
        keyboard[keyboard.size() - 1].push_back(
            detail::makeCallbackButton(utils::utf8str(u8"🤤 Хочу кушать!"), "storage_list_what_to_cook"));
    } else {
        keyboard[keyboard.size() - 1].push_back(detail::makeCallbackButton(u8"Список покупок", "shopping_list"));
    }
    auto text = utils::utf8str(u8"🍱 Ваши хранилища");
    if (toBeEdited) {
        auto messageId = message::getMessageId(userId);
        bot.editMessageText(text, chatId, *messageId, "", "", nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
    } else {
        auto message = bot.sendMessage(chatId, text, nullptr, nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
        message::addMessageId(userId, message->messageId);
    }
}

} // namespace cookcookhnya::render::storage_list
