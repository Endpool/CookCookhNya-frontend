#include "delete.hpp"

#include "backend/api/storages.hpp"
#include "backend/id_types.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils/utils.hpp"

#include <utility>

namespace cookcookhnya::render::delete_storage {

void renderStorageDeletion(
    api::StorageId storageId, ChatId chatId, BotRef bot, UserId userId, api::StorageApiRef storageApi) {
    auto storage = storageApi.get(userId, storageId);
    InlineKeyboard keyboard(2);
    keyboard[0].push_back(makeCallbackButton(u8"▶️ Подтвердить", "confirm"));
    keyboard[1].push_back(makeCallbackButton(u8"↩️ Назад", "back"));
    auto text = utils::utf8str(u8"🚮 Вы уверены, что хотите удалить хранилище?");
    auto messageId = message::getMessageId(userId);
    if (messageId)
        bot.editMessageText(text, chatId, *messageId, makeKeyboardMarkup(std::move(keyboard)));
};

} // namespace cookcookhnya::render::delete_storage
