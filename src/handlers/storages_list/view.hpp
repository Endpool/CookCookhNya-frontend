#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::storages_list {

void handleStorageListCQ(StorageList&, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api);

} // namespace cookcookhnya::handlers::storages_list
