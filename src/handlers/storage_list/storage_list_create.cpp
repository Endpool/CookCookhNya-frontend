#include "storage_list_create.hpp"

#include "backend/models/storage.hpp"
#include "handlers/common.hpp"
#include "render/storage_list/storage_list_render.hpp"

namespace cookcookhnya::handlers::storage_create {

using namespace render::storage_list;

void createStorage(StorageCreationEnterName& /*unused*/,
                   MessageRef m,
                   BotRef bot,
                   SMRef stateManager,
                   StorageApiRef storageApi) {                                      // BackendProvider bkn (Max: bkn?)
    storageApi.create(m.from->id, api::models::storage::StorageCreateBody{m.text}); // Create storage body with new name
    stateManager.put(StorageList{});
    renderStorageList(m.from->id, m.chat->id, bot, storageApi);
};

void cancelStorageCreation(StorageCreationEnterName& /*unused*/,
                           CallbackQueryRef cq,
                           BotRef bot,
                           SMRef stateManager,
                           StorageApiRef storageApi) {
    bot.answerCallbackQuery(cq.id);
    if (cq.data == "cancel_storage_creation") { // Here compare with data in button which was pressed (data was put in
                                            // renderStorageCreate)
        stateManager.put(StorageList{});
        renderStorageList(cq.from->id, cq.message->chat->id, bot, storageApi);
    }
};

} // namespace cookcookhnya::handlers::storage_create
