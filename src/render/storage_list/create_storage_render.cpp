#include "create_storage_render.hpp"

#include "render/common.hpp"

namespace cookcookhnya::render::create_storage {

void renderStorageCreate(ChatId chatId, BotRef bot) { // BackendProvider bkn
    InlineKeyboard keyboard(1);
    keyboard[0].push_back(
        detail::makeCallbackButton(utils::utf8str(u8"Отмена"), "cancel_storage_creation")); // StorageCreateCancel
    bot.sendMessage(chatId,
                    utils::utf8str(u8"Введите имя для нового хранилища"),
                    nullptr,
                    nullptr,
                    detail::makeKeyboardMarkup(std::move(keyboard)));
};

} // namespace cookcookhnya::render::create_storage
