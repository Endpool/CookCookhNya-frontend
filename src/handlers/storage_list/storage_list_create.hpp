#pragma once

#include "handlers/type_refs.hpp"

namespace cookcookhnya::handlers::storageListCreate {
using namespace cookcookhnya::forHandlers;

void createStorage(StorageCreationEnterName&, MessageRef m, BotRef bot, SMRef stateManager, StorageApiRef storageApi);

void cancelStorageCreation(
    StorageCreationEnterName&, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi);
} // namespace cookcookhnya::handlers::storageListCreate
