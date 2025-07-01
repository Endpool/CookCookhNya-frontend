#include "delete_storage_render.hpp"

#include "message_tracker.hpp"
#include "render/common.hpp"

namespace cookcookhnya::render::delete_storage {

void renderStorageDelete(ChatId chatId, BotRef bot, UserId userId, StorageApiRef storageApi) {
    auto storages = storageApi.getStoragesList(userId);
    InlineKeyboard keyboard(storages.size() + 1);
    keyboard[0].push_back(detail::makeCallbackButton(utils::utf8str(u8"Отмена"), "cancel_storage_deletion"));
    for (size_t i = 0; i < storages.size(); i++) {
        if (userId == storages[i].ownerId) {
            keyboard[i + 1].push_back(
                detail::makeCallbackButton(storages[i].name, "st__" + std::to_string(storages[i].id)));
        }
    }
    auto text = utils::utf8str(u8"Что вы хотите удалить");
    auto messageId = cookcookhnya::message::getMessageId(userId);
    bot.editMessageText(text, chatId, *messageId, "", "", nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
};

} // namespace cookcookhnya::render::delete_storage
