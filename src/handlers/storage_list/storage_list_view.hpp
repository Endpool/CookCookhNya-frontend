#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::storage_list_view {

void storageListButtonCallback(
    StorageList&, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi);

} // namespace cookcookhnya::handlers::storage_list_view
