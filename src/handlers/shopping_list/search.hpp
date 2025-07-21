#pragma once

#include "backend/api/api.hpp"
#include "handlers/common.hpp"

#include <cstddef>

namespace cookcookhnya::handlers::shopping_list {

const std::size_t searchPageSize = 10;

void handleShoppingListIngredientSearchCQ(
    ShoppingListIngredientSearch&, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::ApiClientRef api);

void handleShoppingListIngredientSearchIQ(ShoppingListIngredientSearch& state,
                                          InlineQueryRef iq,
                                          BotRef bot,
                                          api::IngredientsApiRef api);

} // namespace cookcookhnya::handlers::shopping_list
