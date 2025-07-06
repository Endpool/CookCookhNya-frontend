#include "storage_view_render.hpp"

#include "backend/id_types.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils.hpp"

#include <cstddef>
#include <utility>

namespace cookcookhnya::render::storage {

void renderStorageView(api::StorageId storageId, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi) {
    auto storage = storageApi.get(userId, storageId);
    const std::size_t buttonRows = 2;
    InlineKeyboard keyboard(buttonRows);
    keyboard[0].reserve(3);
    keyboard[1].reserve(1);
    keyboard[0].push_back(detail::makeCallbackButton(u8"↩️ Назад", "storage_view_back"));
    keyboard[0].push_back(detail::makeCallbackButton(u8"👥 Участники", "storage_view_members"));
    keyboard[0].push_back(detail::makeCallbackButton(u8"🍗 Продукты", "storage_view_explore"));
    keyboard[1].push_back(detail::makeCallbackButton(u8"😋 Хочу кушать!", "storage_view_wanna_eat"));
    auto text = utils::utf8str(u8"Вы находитесь в хранилище  🍱 ") + storage.name + "\n";
    auto messageId = message::getMessageId(userId);
    if (messageId) {
        bot.editMessageText(text, chatId, *messageId, "", "", nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
    }
}

} // namespace cookcookhnya::render::storage
