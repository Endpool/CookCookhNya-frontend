#pragma once

#include "../typeRefs.hpp"

namespace handlers::storageListDelete {
using namespace cookcookhnya::forHandlers;
    
extern void deleteStorage(StorageDeletionEnterName&,
                          CallbackQueryRef cq,
                          BotRef bot,
                          SMRef stateManager, BackendApiRef api);
using storgeDeleteHandler = Handler<Events::CallbackQuery{}, deleteStorage>;

extern void cancelStorageDeletion(StorageDeletionEnterName&, CallbackQueryRef cq, BotRef bot, SMRef stateManager, BackendApiRef api);
using StorageDeleteButtonHandler = Handler<Events::CallbackQuery{}, cancelStorageDeletion>;
}  // namespace handlers::storageListDelete
