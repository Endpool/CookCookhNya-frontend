#include "storageViewRender.hpp"

namespace cookcookhnya::render::viewStorage{
using namespace cookcookhnya::render;

void renderStorageView(StorageId storageId, UserId userId, ChatId chatId, BotRef bot, BackendApiRef api) {
    auto storage = api.get(userId, storageId);
    unsigned int buttonRows = 3;
    InlineKeyboard keyboard(1);
    keyboard[0].reserve(3);
    keyboard[0].push_back(detail::makeCallbackButton("Explore", "explore"));
    keyboard[0].push_back(detail::makeCallbackButton("Members", "members"));
    keyboard[0].push_back(detail::makeCallbackButton("Back", "back"));

    bot.sendMessage(chatId,
                    storage.name,
                    nullptr,
                    nullptr,
                    detail::makeKeyboardMarkup(std::move(keyboard)));
}

}  // namespace cookcookhnya::render::viewStorage
