#include "createStorageRender.hpp"

namespace render::createStorage{
using namespace cookcookhnya::render;

void renderStorageCreate(ChatId chatId, BotRef bot, UserId userId) { // BackendProvider bkn
    InlineKeyboard keyboard(1);
    keyboard[0].push_back(detail::makeCallbackButton("Cancel", "StorageCreateCancel"));
    bot.sendMessage(
        chatId, "Enter new storage name", nullptr, nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
};

} // namespace render::createStorage
