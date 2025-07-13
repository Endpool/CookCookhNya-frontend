#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::recipe {

void handleRecipeAddStorage(
    RecipeAddStoradge& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api);

} // namespace cookcookhnya::handlers::recipe
