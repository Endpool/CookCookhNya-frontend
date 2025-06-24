#pragma once

#include "handlers/type_refs.hpp"

namespace cookcookhnya::handlers::storageAddDeleteMembers {
using namespace cookcookhnya::handlers;

void addDeleteMember(
    MembersAdditionDeletion& state, MessageRef m, BotRef bot, SMRef stateManager, StorageApiRef storageApi);

void cancelAddDeleteMember(
    MembersAdditionDeletion& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi);
} // namespace cookcookhnya::handlers::storageAddDeleteMembers
