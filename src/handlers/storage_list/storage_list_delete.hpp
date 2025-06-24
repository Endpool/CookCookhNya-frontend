#pragma once

#include "handlers/type_refs.hpp"

namespace cookcookhnya::handlers::storageListDelete {
using namespace cookcookhnya::handlers;

void deleteStorage(
    StorageDeletionEnterName&, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi);

void cancelStorageDeletion(
    StorageDeletionEnterName&, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi);
} // namespace cookcookhnya::handlers::storageListDelete
