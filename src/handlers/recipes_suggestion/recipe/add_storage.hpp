#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::recipe_add_storage {

void handleRecipeAddStorage(
    RecipeAddStoarege& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api);

} // namespace cookcookhnya::handlers::recipe_add_storage
