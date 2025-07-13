#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::shopping_list {

void handleShoppingListCreationCQ(
    ShoppingListCreation& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api);

} // namespace cookcookhnya::handlers::shopping_list
