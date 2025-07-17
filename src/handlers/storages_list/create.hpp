#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::storages_list {

void handleStorageCreationEnterNameMsg(
    StorageCreationEnterName&, MessageRef m, BotRef bot, SMRef stateManager, StorageApiRef storageApi);

void handleStorageCreationEnterNameCQ(
    StorageCreationEnterName&, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi);

} // namespace cookcookhnya::handlers::storages_list
