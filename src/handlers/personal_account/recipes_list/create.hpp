#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::personal_account::recipes {

void createRecipe(CreateCustomRecipe&, MessageRef m, BotRef bot, SMRef stateManager, RecipesApiRef recipeApi);

void cancelRecipeCreation(
    CreateCustomRecipe&, CallbackQueryRef cq, BotRef bot, SMRef stateManager, RecipesApiRef recipeApi);

} // namespace cookcookhnya::handlers::personal_account::recipes
