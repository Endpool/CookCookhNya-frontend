#include "view.hpp"

#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils/to_string.hpp"
#include "utils/utils.hpp"

#include <cstddef>
#include <utility>

namespace cookcookhnya::render::storages_list {

using namespace tg_types;

void renderStorageList(bool toBeEdited, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi) {
    auto storages = storageApi.getStoragesList(userId);

    const std::size_t buttonRows = storages.empty() ? 2 : ((storages.size() + 2) / 2) + 1;
    InlineKeyboard keyboard(buttonRows);

    for (std::size_t i = 0; i < storages.size(); i++) {
        if (i % 2 == 0)
            keyboard[i / 2].reserve(2);
        keyboard[i / 2].push_back(makeCallbackButton("🍱 " + storages[i].name, utils::to_string(storages[i].id)));
    }
    keyboard[storages.size() / 2].push_back(makeCallbackButton(u8"🆕 Создать", "create"));
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

} // namespace cookcookhnya::render::storages_list
