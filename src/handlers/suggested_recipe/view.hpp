#pragma once

#include "backend/api/api.hpp"
#include "handlers/common.hpp"

namespace cookcookhnya::handlers::suggested_recipe {

void handleRecipeViewCQ(
    SuggestedRecipeView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::ApiClientRef api);

} // namespace cookcookhnya::handlers::suggested_recipe
