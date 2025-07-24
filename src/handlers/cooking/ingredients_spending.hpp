#pragma once

#include "backend/api/api.hpp"
#include "handlers/common.hpp"

namespace cookcookhnya::handlers::cooking {

void handleCookingIngredientsSpendingCQ(
    CookingIngredientsSpending& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::ApiClientRef api);

} // namespace cookcookhnya::handlers::cooking
