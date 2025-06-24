#include "storage_list_create.hpp"
#include "handlers/type_refs.hpp"
#include "render/storage_list/storage_list_render.hpp"

namespace cookcookhnya::handlers::storageListCreate {

using namespace cookcookhnya::render::storageList;
using namespace cookcookhnya::handlers;

void createStorage(StorageCreationEnterName&,
                   MessageRef m,
                   BotRef bot,
                   SMRef stateManager,
                   StorageApiRef storageApi) {                                 // BackendProvider bkn
    storageApi.create(m.from->id, models::storage::StorageCreateBody{m.text}); // Create storage body with new name
    stateManager.put(StorageList{});
    renderStorageList(m.from->id, m.chat->id, bot, storageApi);
};

void cancelStorageCreation(
    StorageCreationEnterName&, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi) {
    bot.answerCallbackQuery(cq.id);
    if (cq.data == "StorageCreateCancel") { // Here compare with data in button which was pressed (data was put in
                                            // renderStorageCreate)
        stateManager.put(StorageList{});
        renderStorageList(cq.from->id, cq.message->chat->id, bot, storageApi);
    }
};

} // namespace cookcookhnya::handlers::storageListCreate
