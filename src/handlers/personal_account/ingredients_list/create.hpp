#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::personal_account::ingredients {

void handleCustomIngredientCreationEnterNameMsg(CustomIngredientCreationEnterName& /*unused*/,
                                                MessageRef m,
                                                BotRef& bot,
                                                SMRef stateManager,
                                                IngredientsApiRef api);

void handleCustomIngredientCreationEnterNameCQ(CustomIngredientCreationEnterName& /*unused*/,
                                               CallbackQueryRef cq,
                                               BotRef& bot,
                                               SMRef stateManager,
                                               IngredientsApiRef api);

void handleCustomIngredientConfirmationCQ(
    CustomIngredientConfirmation& /*unused*/, CallbackQueryRef cq, BotRef& bot, SMRef stateManager, ApiClientRef api);

} // namespace cookcookhnya::handlers::personal_account::ingredients
