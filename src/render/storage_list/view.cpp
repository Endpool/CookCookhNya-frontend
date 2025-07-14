#include "view.hpp"

#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils/utils.hpp"

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
        keyboard[0].push_back(makeCallbackButton(u8"🆕 Создать", "create"));
        keyboard[0].push_back(makeCallbackButton(u8"🚮 Удалить", "delete"));
    } else {
        keyboard[0].push_back(makeCallbackButton(u8"🆕 Создать", "create"));
    }

    for (std::size_t i = 0; i < storages.size(); i++) {
        if (i % 2 == 0)
            keyboard[1 + (i / 2)].reserve(2);
        keyboard[1 + (i / 2)].push_back(makeCallbackButton("🍱 " + storages[i].name, utils::to_string(storages[i].id)));
    }
    keyboard[buttonRows - 1].push_back(makeCallbackButton(u8"↩️ Назад", "back"));
    auto text = utils::utf8str(u8"🍱 Ваши хранилища");
    if (toBeEdited) {
        auto messageId = message::getMessageId(userId);
        if (messageId) {
            bot.editMessageText(text, chatId, *messageId, makeKeyboardMarkup(std::move(keyboard)));
        }
    } else {
        auto message = bot.sendMessage(chatId, text, makeKeyboardMarkup(std::move(keyboard)));
        message::addMessageId(userId, message->messageId);
    }
}

} // namespace cookcookhnya::render::storage_list
