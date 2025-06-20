#pragma once

#include "../typeRefs.hpp"

namespace handlers::storageAddDeleteMembers {
using namespace cookcookhnya::forHandlers;

extern void addDeleteMember(MembersAdditionDeletion& state, MessageRef m, BotRef bot, SMRef stateManager, BackendApiRef api);

extern void cancelAddDeleteMember(MembersAdditionDeletion& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, BackendApiRef api);
} // namespace handlers::storageAddDeleteMembers