#pragma once

#include "backend/api/api.hpp"
#include "handlers/common.hpp"

namespace cookcookhnya::handlers::recipes_suggestions {

void handleSuggestedRecipesListCQ(
    SuggestedRecipesList& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::ApiClientRef api);

} // namespace cookcookhnya::handlers::recipes_suggestions
