#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::storage::ingredients {

void handleStorageIngredientsDeletionCQ(
    StorageIngredientsDeletion& state, CallbackQueryRef cq, BotRef& bot, SMRef stateManager, ApiClientRef api);

} // namespace cookcookhnya::handlers::storage::ingredients
