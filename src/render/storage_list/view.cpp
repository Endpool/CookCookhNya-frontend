#include "view.hpp"

#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils.hpp"

#include <cstddef>
#include <string>
#include <utility>

namespace cookcookhnya::render::storage_list {

using namespace tg_types;

void renderStorageList(bool toBeEdited, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi) {

    auto storages = storageApi.getStoragesList(userId);

    const std::size_t buttonRows = storages.empty() ? 2 : ((storages.size() + 1) / 2) + 2; // ceiling
    InlineKeyboard keyboard(buttonRows);

    if (!storages.empty()) {
        keyboard[0].reserve(2);
        keyboard[0].push_back(detail::makeCallbackButton(u8"🆕 Создать", "storage_list_creation"));
        keyboard[0].push_back(detail::makeCallbackButton(u8"🚮 Удалить", "storage_list_deletion"));
    } else {
        keyboard[0].reserve(1);
        keyboard[0].push_back(detail::makeCallbackButton(u8"🆕 Создать", "storage_list_creation"));
    }

    for (std::size_t i = 0; i < storages.size(); i++) {
        if (i % 2 == 0)
            keyboard[1 + (i / 2)].reserve(2);
        keyboard[1 + (i / 2)].push_back(
            detail::makeCallbackButton("🍱 " + storages[i].name, std::to_string(storages[i].id)));
    }
    keyboard[buttonRows - 1].push_back(detail::makeCallbackButton(u8"↩️ Назад", "back"));
    auto text = utils::utf8str(u8"🍱 Ваши хранилища");
    if (toBeEdited) {
        auto messageId = message::getMessageId(userId);
        if (messageId) {
            bot.editMessageText(
                text, chatId, *messageId, "", "", nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
        }
    } else {
        auto message = bot.sendMessage(chatId, text, nullptr, nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
        message::addMessageId(userId, message->messageId);
    }
}

} // namespace cookcookhnya::render::storage_list
