#pragma once

#include "handlers/type_refs.hpp"

namespace cookcookhnya::handlers::storageViewMembers {
using namespace cookcookhnya::forHandlers;

void storageMemberViewButtonCallback(
    StorageMemberView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, BackendApiRef api);
} // namespace cookcookhnya::handlers::storageViewMembers
