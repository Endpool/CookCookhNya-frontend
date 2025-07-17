#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::recipe {

void handleRecipeStorageAdditionCQ(
    RecipeStorageAddition& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api);

} // namespace cookcookhnya::handlers::recipe
