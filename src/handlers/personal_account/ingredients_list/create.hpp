#pragma once

#include "backend/api/api.hpp"
#include "backend/api/ingredients.hpp"
#include "handlers/common.hpp"

namespace cookcookhnya::handlers::personal_account::ingredients {

void handleCustomIngredientCreationEnterNameMsg(CustomIngredientCreationEnterName& /*unused*/,
                                                MessageRef m,
                                                BotRef& bot,
                                                SMRef stateManager,
                                                api::IngredientsApiRef api);

void handleCustomIngredientCreationEnterNameCQ(CustomIngredientCreationEnterName& /*unused*/,
                                               CallbackQueryRef cq,
                                               BotRef& bot,
                                               SMRef stateManager,
                                               api::IngredientsApiRef api);

void handleCustomIngredientConfirmationCQ(CustomIngredientConfirmation& /*unused*/,
                                          CallbackQueryRef cq,
                                          BotRef& bot,
                                          SMRef stateManager,
                                          api::ApiClientRef api);

} // namespace cookcookhnya::handlers::personal_account::ingredients
