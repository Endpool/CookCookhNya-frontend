#pragma once

#include "backend/api/storages.hpp"
#include "handlers/common.hpp"

namespace cookcookhnya::handlers::storage::members {

void handleStorageMemberAdditionMsg(
    StorageMemberAddition& state, MessageRef m, BotRef bot, SMRef stateManager, api::StorageApiRef storageApi);

void handleStorageMemberAdditionCQ(
    StorageMemberAddition& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::StorageApiRef storageApi);

} // namespace cookcookhnya::handlers::storage::members
