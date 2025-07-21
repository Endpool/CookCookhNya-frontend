#pragma once

#include "backend/api/storages.hpp"
#include "handlers/common.hpp"

namespace cookcookhnya::handlers::storages_list {

void handleStorageDeletionCQ(
    StorageDeletion&, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::StorageApiRef storageApi);

} // namespace cookcookhnya::handlers::storages_list
