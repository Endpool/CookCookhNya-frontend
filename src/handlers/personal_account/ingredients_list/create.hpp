#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::personal_account::ingredients {

void customIngredientEnterName(CustomIngredientCreationEnterName& /*unused*/,
                               MessageRef m,
                               BotRef& bot,
                               SMRef stateManager,
                               IngredientsApiRef api);

void customIngredientCancelCreation(CustomIngredientCreationEnterName& /*unused*/,
                                    CallbackQueryRef cq,
                                    BotRef& bot,
                                    SMRef stateManager,
                                    IngredientsApiRef api);

void customIngredientConfirmation(CustomIngredientConfirmation& /*unused*/,
                                  CallbackQueryRef cq,
                                  BotRef& bot,
                                  SMRef stateManager,
                                  IngredientsApiRef api);

} // namespace cookcookhnya::handlers::personal_account::ingredients
