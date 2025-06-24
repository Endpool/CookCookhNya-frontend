#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::storages_select {

void selectStorages(StorageSelection& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api);

} // namespace cookcookhnya::handlers::storages_select
