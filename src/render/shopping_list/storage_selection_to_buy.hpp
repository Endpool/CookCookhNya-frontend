#pragma once

#include "render/common.hpp"
#include "states.hpp"

namespace cookcookhnya::render::shopping_list {

void renderShoppingListStorageSelectionToBuy(const states::ShoppingListStorageSelectionToBuy& state,
                                             UserId userId,
                                             ChatId chatId,
                                             BotRef bot);

} // namespace cookcookhnya::render::shopping_list
