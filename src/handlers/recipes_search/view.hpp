#pragma once

#include "backend/api/api.hpp"
#include "backend/api/recipes.hpp"
#include "handlers/common.hpp"

namespace cookcookhnya::handlers::recipes_search {

void handleRecipesSearchCQ(
    RecipesSearch& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::ApiClientRef api);

void handleRecipesSearchIQ(RecipesSearch& state, InlineQueryRef iq, BotRef bot, api::RecipesApiRef api);

} // namespace cookcookhnya::handlers::recipes_search
