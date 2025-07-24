#pragma once

#include "backend/api/storages.hpp"
#include "handlers/common.hpp"

namespace cookcookhnya::handlers::storage {

void handleStorageDeletionCQ(
    StorageDeletion&, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::StorageApiRef storageApi);

} // namespace cookcookhnya::handlers::storage
