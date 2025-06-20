#include "storageListView.hpp"
#include "../../render/storageList/createStorageRender.hpp"
#include "../../render/storageList/deleteStorageRender.hpp"
#include "../../render/storageView/storageViewRender.hpp"

namespace handlers::storageListView {
    
using namespace cookcookhnya::forHandlers;
using namespace render::createStorage;
using namespace render::deleteStorage;
using namespace render::viewStorage;

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
        renderStorageCreate(chatId, bot, cq.from->id);             // Bot here prints menu of storage creation ADD USER ID
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

}  // namespace handlers::storageListView