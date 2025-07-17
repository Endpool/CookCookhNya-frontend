#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::personal_account::ingredients {

void handleCustomIngredientPublishCQ(
    CustomIngredientPublish& /*unused*/, CallbackQueryRef cq, BotRef& bot, SMRef stateManager, IngredientsApiRef api);

} // namespace cookcookhnya::handlers::personal_account::ingredients
