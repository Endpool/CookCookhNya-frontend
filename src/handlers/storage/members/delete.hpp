#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::storage::members {

void handleStorageMemberDeletionCQ(
    StorageMemberDeletion& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi);

} // namespace cookcookhnya::handlers::storage::members
