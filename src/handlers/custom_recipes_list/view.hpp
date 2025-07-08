#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::custom_recipes_list {

void customRecipeList(CustomRecipesList& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api);

} // namespace cookcookhnya::handlers::custom_recipes_list
