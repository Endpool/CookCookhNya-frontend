#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::storages_list {

void deleteStorage(StorageDeletion&, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi);

} // namespace cookcookhnya::handlers::storages_list
