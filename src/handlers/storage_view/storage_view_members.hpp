#pragma once

#include "handlers/type_refs.hpp"

namespace cookcookhnya::handlers::storageViewMembers {
using namespace cookcookhnya::handlers;

void storageMemberViewButtonCallback(
    StorageMemberView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi);
} // namespace cookcookhnya::handlers::storageViewMembers
