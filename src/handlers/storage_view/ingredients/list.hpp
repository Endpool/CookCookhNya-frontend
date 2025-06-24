#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::storage::ingredients {

void storageIngredientsListButtonCallback(
    StorageIngredientsView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api);

} // namespace cookcookhnya::handlers::storage::ingredients
