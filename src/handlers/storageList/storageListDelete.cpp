#include "storageListDelete.hpp"
#include "../../render/storageList/storageListRender.hpp"

namespace cookcookhnya::handlers::storageListDelete {

using namespace cookcookhnya::render::storageList;
using namespace cookcookhnya::forHandlers;

void deleteStorage(StorageDeletionEnterName&,
                          CallbackQueryRef cq,
                          BotRef bot,
                          SMRef stateManager, BackendApiRef api) { 

    std::stringstream temp;
    temp << cq.data;
    StorageId id = 0;
    temp >> id; // Probably dangerous (However can be okay as storageId is long int)
    api.delete_(cq.from->id, id); // delete by a userid and id of their storage
    stateManager.put(StorageList{}); // As button was pressed return to storage list view
    renderStorageList(cq.from->id,cq.message->chat->id, bot, api);
};

void cancelStorageDeletion(StorageDeletionEnterName&, CallbackQueryRef cq, BotRef bot, SMRef stateManager, BackendApiRef api) {
    bot.answerCallbackQuery(cq.id);
    if (cq.data == "cancel") {
        stateManager.put(StorageList{});
        renderStorageList(cq.from->id, cq.message->chat->id, bot, api);
    }
};

}  // namespace cookcookhnya::handlers::storageListDelete
