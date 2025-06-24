#include "storage_list_delete.hpp"

#include "handlers/type_refs.hpp"
#include "render/storage_list/storage_list_render.hpp"

namespace cookcookhnya::handlers::storage_delete {

using namespace render::storageList;

void deleteStorage(StorageDeletionEnterName& /*unused*/,
                   CallbackQueryRef cq,
                   BotRef bot,
                   SMRef stateManager,
                   StorageApiRef storageApi) {

    std::stringstream temp;
    temp << cq.data;
    StorageId id = 0;
    temp >> id;                          // Probably dangerous (However can be okay as storageId is long int)
    storageApi.delete_(cq.from->id, id); // delete by a userid and id of their storage
    stateManager.put(StorageList{});     // As button was pressed return to storage list view
    renderStorageList(cq.from->id, cq.message->chat->id, bot, storageApi);
};

void cancelStorageDeletion(StorageDeletionEnterName& /*unused*/,
                           CallbackQueryRef cq,
                           BotRef bot,
                           SMRef stateManager,
                           StorageApiRef storageApi) {
    bot.answerCallbackQuery(cq.id);
    if (cq.data == "cancel") {
        stateManager.put(StorageList{});
        renderStorageList(cq.from->id, cq.message->chat->id, bot, storageApi);
    }
};

} // namespace cookcookhnya::handlers::storage_delete
