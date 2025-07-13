#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::storages_list {

void handleStorageDeletionCQ(
    StorageDeletion&, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi);

} // namespace cookcookhnya::handlers::storages_list
