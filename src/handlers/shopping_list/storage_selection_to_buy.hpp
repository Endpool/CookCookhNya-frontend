#pragma once

#include "backend/api/shopping_lists.hpp"
#include "handlers/common.hpp"

namespace cookcookhnya::handlers::shopping_list {

void handleShoppingListStorageSelectionToBuyCQ(ShoppingListStorageSelectionToBuy& state,
                                               CallbackQueryRef cq,
                                               BotRef bot,
                                               SMRef stateManager,
                                               api::ShoppingListApiRef api);

} // namespace cookcookhnya::handlers::shopping_list
