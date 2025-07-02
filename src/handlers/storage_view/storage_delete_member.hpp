#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::storage_delete_member {

void deleteMember(MemberDeletion& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi);

} // namespace cookcookhnya::handlers::storage_delete_member
