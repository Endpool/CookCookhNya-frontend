#pragma once

#include "../typeRefs.hpp"

namespace handlers::storageViewMembers {
using namespace cookcookhnya::forHandlers;

extern void storageMemberViewButtonCallback(StorageMemberView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager,  BackendApiRef api);
}  // namespace handlers::storageViewMembers
