#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::storage {

void handleStorageViewCQ(StorageView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api);

} // namespace cookcookhnya::handlers::storage
