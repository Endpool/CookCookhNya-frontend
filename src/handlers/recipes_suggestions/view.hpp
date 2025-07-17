#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::recipes_suggestions {

void handleSuggestedRecipeListCQ(
    SuggestedRecipeList& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api);

} // namespace cookcookhnya::handlers::recipes_suggestions
