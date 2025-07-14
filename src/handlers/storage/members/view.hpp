#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::storage::members {

void handleStorageMemberViewCQ(
    StorageMemberView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi);

} // namespace cookcookhnya::handlers::storage::members
