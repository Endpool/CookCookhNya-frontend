#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::shopping_list_create {

void handleProductListSubmission(
    ShoppingListCreation& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api);

} // namespace cookcookhnya::handlers::shopping_list_create
