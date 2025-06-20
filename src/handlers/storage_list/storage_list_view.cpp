#include "storage_list_view.hpp"
#include "render/storage_list/create_storage_render.hpp"
#include "render/storage_list/delete_storage_render.hpp"
#include "render/storage_view/storage_view_render.hpp"

namespace cookcookhnya::handlers::storageListView {

using namespace cookcookhnya::forHandlers;
using namespace cookcookhnya::render::createStorage;
using namespace cookcookhnya::render::deleteStorage;
using namespace cookcookhnya::render::viewStorage;

void storageListButtonCallback(StorageList&,
                                      CallbackQueryRef cq,
                                      const Api& bot,
                                      SMRef stateManager, BackendApiRef api) {
    bot.answerCallbackQuery(cq.id);

    std::stringstream temp; // Convert string to int
    temp << cq.data;
    int id = 0;
    temp >> id;

    auto chatId = cq.message->chat->id;
    if (cq.data == "StorageViewCreate") {
        stateManager.put(StorageCreationEnterName{}); // Go to function create storage, while cancel button is handled
                                                      // on cancel storage creation
        renderStorageCreate(chatId, bot);             // Bot here prints menu of storage creation ADD USER ID
        return;
    }

    if (cq.data == "StorageViewDelete") {
        stateManager.put(StorageDeletionEnterName{});
        renderStorageDelete(chatId, bot, cq.from->id, api); // Need for api, so it could put the list of storages to delete
        return;
    }

    stateManager.put(StorageView{id});
    renderStorageView(id, cq.from->id, chatId, bot, api); // If nor buttons were pressed then user pressed on their storages
}

}  // namespace cookcookhnya::handlers::storageListView
