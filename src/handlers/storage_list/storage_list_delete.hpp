#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::storage_delete {

void deleteStorage(
    StorageDeletionEnterName&, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi);

void cancelStorageDeletion(
    StorageDeletionEnterName&, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi);

} // namespace cookcookhnya::handlers::storage_delete
