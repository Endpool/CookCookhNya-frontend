#pragma once

#include "backend/api/api.hpp"
#include "handlers/common.hpp"

namespace cookcookhnya::handlers::personal_account::ingredients {

void handleCustomIngredientsListCQ(
    CustomIngredientsList& /*unused*/, CallbackQueryRef cq, BotRef& bot, SMRef stateManager, api::ApiClientRef api);

} // namespace cookcookhnya::handlers::personal_account::ingredients
