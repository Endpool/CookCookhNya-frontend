#pragma once

#include "backend/api/storages.hpp"
#include "handlers/common.hpp"

namespace cookcookhnya::handlers::storages_list {

void handleStorageCreationEnterNameMsg(
    StorageCreationEnterName&, MessageRef m, BotRef bot, SMRef stateManager, api::StorageApiRef storageApi);

void handleStorageCreationEnterNameCQ(
    StorageCreationEnterName&, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::StorageApiRef storageApi);

} // namespace cookcookhnya::handlers::storages_list
