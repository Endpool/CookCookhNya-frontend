#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::storage::ingredients {

void storageIngredientsListButtonCallback(
    StorageIngredientsList& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api);

} // namespace cookcookhnya::handlers::storage::ingredients
