#include "storage_view_render.hpp"
#include "render/types_for_render.hpp"

namespace cookcookhnya::render::viewStorage {
using namespace cookcookhnya::render;

void renderStorageView(StorageId storageId, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi) {
    auto storage = storageApi.get(userId, storageId);
    unsigned int buttonRows = 3;
    InlineKeyboard keyboard(buttonRows);
    keyboard[0].reserve(3);
    keyboard[0].push_back(detail::makeCallbackButton("Explore", "explore"));
    keyboard[0].push_back(detail::makeCallbackButton("Members", "members"));
    keyboard[0].push_back(detail::makeCallbackButton("Back", "back"));

    bot.sendMessage(chatId, storage.name, nullptr, nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
}

} // namespace cookcookhnya::render::viewStorage
