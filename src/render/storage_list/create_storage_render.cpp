#include "create_storage_render.hpp"

#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils.hpp"

#include <utility>

namespace cookcookhnya::render::create_storage {

void renderStorageCreate(ChatId chatId, UserId userId, BotRef bot) { // BackendProvider bkn
    InlineKeyboard keyboard(1);
    keyboard[0].push_back(
        detail::makeCallbackButton(utils::utf8str(u8"🚫 Отмена"), "cancel_storage_creation")); // StorageCreateCancel
    auto text = utils::utf8str(u8"🏷 Введите новое имя хранилища");
    auto messageId = message::getMessageId(userId);
    bot.editMessageText(text, chatId, *messageId, "", "", nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
};

} // namespace cookcookhnya::render::create_storage
