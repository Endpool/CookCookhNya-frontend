#pragma once

#include "handlers/type_refs.hpp"

namespace cookcookhnya::handlers::storage_view_members {

void storageMemberViewButtonCallback(
    StorageMemberView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi);

} // namespace cookcookhnya::handlers::storage_view_members
