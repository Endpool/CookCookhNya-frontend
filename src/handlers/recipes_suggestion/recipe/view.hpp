#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::recipe_view {

void handleRecipeView(RecipeView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api);

} // namespace cookcookhnya::handlers::recipe_view
