#include "delete.hpp"

#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils/utils.hpp"

#include <cstddef>
#include <format>
#include <string>
#include <utility>

namespace cookcookhnya::render::delete_storage {

void renderStorageDeletion(ChatId chatId, BotRef bot, UserId userId, StorageApiRef storageApi) {
    auto storages = storageApi.getStoragesList(userId);
    size_t numStoragesOwner = 0;
    for (auto& storage : storages) {
        if (userId == storage.ownerId) {
            numStoragesOwner++;
        }
    }
    InlineKeyboard keyboard(numStoragesOwner + 1);
    keyboard[0].push_back(makeCallbackButton(u8"↩️ Назад", "back"));
    size_t k = 1;
    for (auto& storage : storages) {
        if (userId == storage.ownerId) {
            keyboard[k++].push_back(makeCallbackButton(std::format("{} {}", utils::utf8str(u8"🍱"), storage.name),
                                                       "st__" + utils::to_string(storage.id)));
        }
    }
    auto text = utils::utf8str(u8"🚮 Выберите хранилище для удаления");
    auto messageId = message::getMessageId(userId);
    if (messageId)
        bot.editMessageText(text, chatId, *messageId, makeKeyboardMarkup(std::move(keyboard)));
};

} // namespace cookcookhnya::render::delete_storage
