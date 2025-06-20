#pragma once

#include "../typeRefs.hpp"

namespace handlers::storageListCreate {
using namespace cookcookhnya::forHandlers;

extern void createStorage(StorageCreationEnterName&, MessageRef m, BotRef bot, SMRef stateManager, BackendApiRef api);


extern void cancelStorageCreation(StorageCreationEnterName&, CallbackQueryRef cq, BotRef bot, SMRef stateManager, BackendApiRef api);


} // namespace handlers::storageListCreate