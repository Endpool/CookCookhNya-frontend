#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::custom_ingredients_view {

void customIngredientsList(
    CustomIngredientsList& /*unused*/, CallbackQueryRef cq, BotRef& bot, SMRef stateManager, ApiClientRef api);

} // namespace cookcookhnya::handlers::custom_ingredients_view
