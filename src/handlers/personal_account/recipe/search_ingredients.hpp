#pragma once

#include "backend/api/api.hpp"
#include "backend/api/ingredients.hpp"
#include "handlers/common.hpp"

namespace cookcookhnya::handlers::personal_account::recipes {

void handleCustomRecipeIngredientsSearchCQ(
    CustomRecipeIngredientsSearch& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::ApiClientRef api);

void handleCustomRecipeIngredientsSearchIQ(CustomRecipeIngredientsSearch& state,
                                           InlineQueryRef iq,
                                           BotRef bot,
                                           api::IngredientsApiRef api);

} // namespace cookcookhnya::handlers::personal_account::recipes
