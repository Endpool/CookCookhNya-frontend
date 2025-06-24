#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::storage_view {

void storageViewButtonCallback(
    StorageView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi);

} // namespace cookcookhnya::handlers::storage_view
