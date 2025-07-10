#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::recipe_view {

void handleCustomRecipeView(
    RecipeCustomView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, RecipesApiRef recipesApi);

} // namespace cookcookhnya::handlers::recipe_view
