#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::shopping_list {

void handleShoppingListStorageSelectionToBuyCQ(ShoppingListStorageSelectionToBuy& state,
                                               CallbackQueryRef cq,
                                               BotRef bot,
                                               SMRef stateManager,
                                               ShoppingListApiRef api);

} // namespace cookcookhnya::handlers::shopping_list
