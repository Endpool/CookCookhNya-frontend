#include "storage_list_create.hpp"
#include "render/storage_list/storage_list_render.hpp"

namespace cookcookhnya::handlers::storageListCreate {

using namespace cookcookhnya::render::storageList;
using namespace cookcookhnya::forHandlers;

void createStorage(StorageCreationEnterName&, MessageRef m, BotRef bot, SMRef stateManager, BackendApiRef api) { // BackendProvider bkn
    api.create(m.from->id, models::storage::StorageCreateBody{m.text}); // Create storage bpdy with new name
    stateManager.put(StorageList{});
    renderStorageList(m.from->id, m.chat->id, bot, api);
};

void cancelStorageCreation(StorageCreationEnterName&, CallbackQueryRef cq, BotRef bot, SMRef stateManager, BackendApiRef api) {
    bot.answerCallbackQuery(cq.id);
    if (cq.data == "StorageCreateCancel") { // Here compare with data in button which was pressed (data was put in
                                            // renderStorageCreate)
        stateManager.put(StorageList{});
        renderStorageList(cq.from->id, cq.message->chat->id, bot, api);
    }
};

}  // namespace cookcookhnya::handlers::storageListCreate
