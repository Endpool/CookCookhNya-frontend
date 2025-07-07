#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::storage_add_member {

void addMember(MemberAddition& state, MessageRef m, BotRef bot, SMRef stateManager, StorageApiRef storageApi);

void cancelMemberAddition(
    MemberAddition& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi);

} // namespace cookcookhnya::handlers::storage_add_member
