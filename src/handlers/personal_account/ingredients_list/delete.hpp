#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::personal_account::ingredients {

void handleCustomIngredientDeletionCQ(
    CustomIngredientDeletion& /*unused*/, CallbackQueryRef cq, BotRef& bot, SMRef stateManager, IngredientsApiRef api);

} // namespace cookcookhnya::handlers::personal_account::ingredients
