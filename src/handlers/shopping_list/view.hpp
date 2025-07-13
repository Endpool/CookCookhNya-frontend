#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::shopping_list {

void handleShoppingListViewCQ(ShoppingListView&, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api);

} // namespace cookcookhnya::handlers::shopping_list
