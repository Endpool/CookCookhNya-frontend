#pragma once

#include "render/common.hpp"
#include "states.hpp"

namespace cookcookhnya::render::shopping_list {

void renderShoppingList(const states::ShoppingListView::ItemsDb::Set& items, UserId userId, ChatId chatId, BotRef bot);

} // namespace cookcookhnya::render::shopping_list
