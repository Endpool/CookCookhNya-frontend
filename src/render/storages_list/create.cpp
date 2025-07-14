#include "create.hpp"

#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils.hpp"

#include <utility>

namespace cookcookhnya::render::create_storage {

void renderStorageCreation(ChatId chatId, UserId userId, BotRef bot) { // BackendProvider bkn
    InlineKeyboard keyboard(1);
    keyboard[0].push_back(makeCallbackButton(u8"↩️ Назад", "back"));
    auto text = utils::utf8str(u8"🏷 Введите новое имя хранилища");
    auto messageId = message::getMessageId(userId);
    if (messageId) {
        bot.editMessageText(text, chatId, *messageId, "", "", nullptr, makeKeyboardMarkup(std::move(keyboard)));
    }
};

} // namespace cookcookhnya::render::create_storage
