#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::publish_custom_ingredients {

void customIngredientPublish(
    CustomIngredientPublish& /*unused*/, CallbackQueryRef cq, BotRef& bot, SMRef stateManager, IngredientsApiRef api);

} // namespace cookcookhnya::handlers::publish_custom_ingredients
