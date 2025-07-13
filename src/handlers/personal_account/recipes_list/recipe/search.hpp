#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::personal_account::recipes {

void customRecipeIngredientsSearchButtonCallback(
    CustomRecipeIngredientsSearch& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api);

void customRecipeIngredientsSearchInlineQueryCallback(CustomRecipeIngredientsSearch& state,
                                                      InlineQueryRef iq,
                                                      BotRef bot,
                                                      IngredientsApiRef api);

} // namespace cookcookhnya::handlers::personal_account::recipes
