#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::storage::ingredients {

void storageIngredientsSearchButtonCallback(
    StorageIngredientsSearch& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, IngredientsApiRef api);

void storageIngredientsSearchInlineQueryCallback(StorageIngredientsSearch& state,
                                                 InlineQueryRef iq,
                                                 BotRef bot,
                                                 IngredientsApiRef api);

} // namespace cookcookhnya::handlers::storage::ingredients
