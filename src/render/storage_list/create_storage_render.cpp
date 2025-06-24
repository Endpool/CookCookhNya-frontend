#include "create_storage_render.hpp"

#include "render/common.hpp"

namespace cookcookhnya::render::create_storage {

void renderStorageCreate(ChatId chatId, BotRef bot) { // BackendProvider bkn
    InlineKeyboard keyboard(1);
    keyboard[0].push_back(detail::makeCallbackButton("Cancel", "StorageCreateCancel"));
    bot.sendMessage(
        chatId, "Enter new storage name", nullptr, nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
};

} // namespace cookcookhnya::render::create_storage
