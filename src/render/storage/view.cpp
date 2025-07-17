#include "view.hpp"

#include "backend/id_types.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils/utils.hpp"

#include <cstddef>
#include <utility>

namespace cookcookhnya::render::storage {

void renderStorageView(api::StorageId storageId, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi) {
    auto storage = storageApi.get(userId, storageId);
    const std::size_t buttonRows = 2;
    InlineKeyboard keyboard(buttonRows);
    keyboard[0].reserve(2);
    keyboard[0].push_back(makeCallbackButton(u8"🍗 Продукты", "ingredients"));
    keyboard[0].push_back(makeCallbackButton(u8"👥 Участники", "members"));
    keyboard[1].reserve(2);
    keyboard[1].push_back(makeCallbackButton(u8"↩️ Назад", "back"));
    keyboard[1].push_back(makeCallbackButton(u8"😋 Хочу кушать!", "wanna_eat"));
    auto text = utils::utf8str(u8"Вы находитесь в хранилище  🍱 ") + storage.name + "\n";
    auto messageId = message::getMessageId(userId);
    if (messageId) {
        bot.editMessageText(text, chatId, *messageId, "", "", nullptr, makeKeyboardMarkup(std::move(keyboard)));
    }
}

} // namespace cookcookhnya::render::storage
