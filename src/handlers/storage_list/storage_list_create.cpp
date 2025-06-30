#include "storage_list_create.hpp"

#include "backend/models/storage.hpp"
#include "handlers/common.hpp"
#include "message_tracker.hpp"
#include "render/storage_list/storage_list_render.hpp"
#include "utils.hpp"

namespace cookcookhnya::handlers::storage_create {

using namespace render::storage_list;

void createStorage(
    StorageCreationEnterName& /*unused*/, MessageRef m, BotRef bot, SMRef stateManager, StorageApiRef storageApi) {
    storageApi.create(m.from->id, api::models::storage::StorageCreateBody{m.text}); // Create storage body with new name
    auto text = utils::utf8str(u8"Введите имя для нового хранилища");
    bot.editMessageText(text, m.chat->id, m.messageId);
    renderStorageList(false, m.from->id, m.chat->id, bot, storageApi);
    stateManager.put(StorageList{});
};

void cancelStorageCreation(StorageCreationEnterName& /*unused*/,
                           CallbackQueryRef cq,
                           BotRef bot,
                           SMRef stateManager,
                           StorageApiRef storageApi) {
    bot.answerCallbackQuery(cq.id);
    if (cq.data == "cancel_storage_creation") {
        renderStorageList(true, cq.from->id, cq.message->chat->id, bot, storageApi);
        stateManager.put(StorageList{});
    }
};

} // namespace cookcookhnya::handlers::storage_create
