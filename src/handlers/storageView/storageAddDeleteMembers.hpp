#pragma once

#include "handlers/typeRefs.hpp"

namespace cookcookhnya::handlers::storageAddDeleteMembers {
using namespace cookcookhnya::forHandlers;

void addDeleteMember(MembersAdditionDeletion& state, MessageRef m, BotRef bot, SMRef stateManager, BackendApiRef api);

void cancelAddDeleteMember(MembersAdditionDeletion& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, BackendApiRef api);
} // namespace cookcookhnya::handlers::storageAddDeleteMembers