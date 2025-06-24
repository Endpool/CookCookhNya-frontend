#include "storage_view_render.hpp"

#include "render/common.hpp"

namespace cookcookhnya::render::view_storage {

void renderStorageView(StorageId storageId, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi) {
    auto storage = storageApi.get(userId, storageId);
    unsigned int buttonRows = 2;
    InlineKeyboard keyboard(buttonRows);
    keyboard[0].reserve(3);
    keyboard[1].reserve(1);
    keyboard[0].push_back(detail::makeCallbackButton(utils::utf8str(u8"Исследовать"), "storage_view_explore"));
    keyboard[0].push_back(detail::makeCallbackButton(utils::utf8str(u8"Участники"), "storage_view_members"));
    keyboard[0].push_back(detail::makeCallbackButton(utils::utf8str(u8"Назад"), "storage_view_back"));
    keyboard[1].push_back(detail::makeCallbackButton(utils::utf8str(u8"Хочу кушать"), "storage_view_what_to_cook"));
    bot.sendMessage(chatId, storage.name, nullptr, nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
}

} // namespace cookcookhnya::render::view_storage
