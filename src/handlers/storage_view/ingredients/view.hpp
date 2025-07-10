#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::storage::ingredients {

void storageIngredientsSearchButtonCallback(
    StorageIngredientsList& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api);

void storageIngredientsSearchInlineQueryCallback(StorageIngredientsList& state,
                                                 InlineQueryRef iq,
                                                 BotRef bot,
                                                 IngredientsApiRef api);

} // namespace cookcookhnya::handlers::storage::ingredients
