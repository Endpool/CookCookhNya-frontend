#pragma once

#include "../typeRefs.hpp"

namespace handlers::storageListDelete {
using namespace cookcookhnya::forHandlers;
    
extern void deleteStorage(StorageDeletionEnterName&,
                          CallbackQueryRef cq,
                          BotRef bot,
                          SMRef stateManager, BackendApiRef api);

extern void cancelStorageDeletion(StorageDeletionEnterName&, CallbackQueryRef cq, BotRef bot, SMRef stateManager, BackendApiRef api);
}  // namespace handlers::storageListDelete
