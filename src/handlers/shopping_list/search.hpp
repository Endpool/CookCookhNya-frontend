#pragma once

#include "handlers/common.hpp"

#include <cstddef>

namespace cookcookhnya::handlers::shopping_list {

const std::size_t searchPageSize = 10;

void handleShoppingListIngredientSearchCQ(
    ShoppingListIngredientSearch&, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api);

void handleShoppingListIngredientSearchIQ(ShoppingListIngredientSearch& state,
                                          InlineQueryRef iq,
                                          BotRef bot,
                                          IngredientsApiRef api);

} // namespace cookcookhnya::handlers::shopping_list
