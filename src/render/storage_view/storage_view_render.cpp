#include "storage_view_render.hpp"

#include "render/common.hpp"

namespace cookcookhnya::render::view_storage {

void renderStorageView(StorageId storageId, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi) {
    auto storage = storageApi.get(userId, storageId);
    unsigned int buttonRows = 3;
    InlineKeyboard keyboard(buttonRows);
    keyboard[0].reserve(1);
    keyboard[1].reserve(1);
    keyboard[2].reserve(1);
    keyboard[0].push_back(detail::makeCallbackButton("Explore", "storage_view_explore")); //explore
    keyboard[1].push_back(detail::makeCallbackButton("Members", "storage_view_members")); // members
    keyboard[2].push_back(detail::makeCallbackButton("Back", "storage_view_back")); //back

    bot.sendMessage(chatId, storage.name, nullptr, nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
}

} // namespace cookcookhnya::render::view_storage
