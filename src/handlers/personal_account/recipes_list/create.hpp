#pragma once

#include "backend/api/recipes.hpp"
#include "handlers/common.hpp"

namespace cookcookhnya::handlers::personal_account::recipes {

void handleCreateCustomRecipeMsg(
    CreateCustomRecipe&, MessageRef m, BotRef bot, SMRef stateManager, api::RecipesApiRef recipeApi);

void handleCreateCustomRecipeCQ(
    CreateCustomRecipe&, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::RecipesApiRef recipeApi);

} // namespace cookcookhnya::handlers::personal_account::recipes
