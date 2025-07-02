#include "delete_storage_render.hpp"

#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils.hpp"

namespace cookcookhnya::render::delete_storage {

void renderStorageDelete(ChatId chatId, BotRef bot, UserId userId, StorageApiRef storageApi) {
    auto storages = storageApi.getStoragesList(userId);
    size_t numStoragesOwner = 0;
    for (auto& storage : storages) {
        if (userId == storage.ownerId) {
            numStoragesOwner++;
        }
    }
    InlineKeyboard keyboard(numStoragesOwner + 1);
    keyboard[0].push_back(detail::makeCallbackButton(utils::utf8str(u8"🚫 Отмена"), "cancel_storage_deletion"));
    size_t k = 1;
    for (auto& storage : storages) {
        if (userId == storage.ownerId) {
            keyboard[k++].push_back(detail::makeCallbackButton(utils::utf8str(u8"🍱 ") + storage.name,
                                                               "st__" + std::to_string(storage.id)));
        }
    }
    auto text = utils::utf8str(u8"🚮 Выберите хранилище для удаления");
    auto messageId = message::getMessageId(userId);
    bot.editMessageText(text, chatId, *messageId, "", "", nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
};

} // namespace cookcookhnya::render::delete_storage
