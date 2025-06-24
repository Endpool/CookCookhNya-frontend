#pragma once

#include "handlers/type_refs.hpp"

namespace cookcookhnya::handlers::storage_list_view {

void storageListButtonCallback(
    StorageList&, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi);

} // namespace cookcookhnya::handlers::storage_list_view
