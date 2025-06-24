#pragma once

#include "handlers/type_refs.hpp"

namespace cookcookhnya::handlers::storage_create {

void createStorage(StorageCreationEnterName&, MessageRef m, BotRef bot, SMRef stateManager, StorageApiRef storageApi);

void cancelStorageCreation(
    StorageCreationEnterName&, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi);

} // namespace cookcookhnya::handlers::storage_create
