#pragma once

#include "backend/api/api.hpp"
#include "handlers/common.hpp"

namespace cookcookhnya::handlers::storage::ingredients {

void handleStorageIngredientsListCQ(
    StorageIngredientsList& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::ApiClientRef api);

void handleStorageIngredientsListIQ(StorageIngredientsList& state,
                                    InlineQueryRef iq,
                                    BotRef bot,
                                    api::IngredientsApiRef api);

} // namespace cookcookhnya::handlers::storage::ingredients
