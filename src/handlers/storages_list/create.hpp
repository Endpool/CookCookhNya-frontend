#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::storages_list {

void createStorage(StorageCreationEnterName&, MessageRef m, BotRef bot, SMRef stateManager, StorageApiRef storageApi);

void cancelStorageCreation(
    StorageCreationEnterName&, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi);

} // namespace cookcookhnya::handlers::storages_list
