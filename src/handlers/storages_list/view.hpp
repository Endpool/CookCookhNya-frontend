#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::storages_list {

void storageListButtonCallback(StorageList&, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api);

} // namespace cookcookhnya::handlers::storages_list
