#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::personal_account::recipes {

void customRecipeList(CustomRecipesList& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api);

} // namespace cookcookhnya::handlers::personal_account::recipes
