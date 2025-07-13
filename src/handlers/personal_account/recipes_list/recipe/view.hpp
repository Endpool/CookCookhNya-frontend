#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::personal_account::recipes {

void handleCustomRecipeView(
    RecipeCustomView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, RecipesApiRef recipesApi);

} // namespace cookcookhnya::handlers::personal_account::recipes
