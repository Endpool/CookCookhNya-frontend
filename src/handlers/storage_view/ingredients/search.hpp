#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::storage::ingredients {

void storageIngredientsSearchButtonCallback(
    StorageIngredientsSearch& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api);

} // namespace cookcookhnya::handlers::storage::ingredients
