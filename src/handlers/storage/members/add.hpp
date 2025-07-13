#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::storage::members {

void addMember(StorageMemberAddition& state, MessageRef m, BotRef bot, SMRef stateManager, StorageApiRef storageApi);

void cancelStorageMemberAddition(
    StorageMemberAddition& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi);

} // namespace cookcookhnya::handlers::storage::members
