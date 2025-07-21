#pragma once

#include "backend/api/storages.hpp"
#include "handlers/common.hpp"

namespace cookcookhnya::handlers::storage::members {

void handleStorageMemberDeletionCQ(
    StorageMemberDeletion& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::StorageApiRef storageApi);

} // namespace cookcookhnya::handlers::storage::members
