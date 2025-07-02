#pragma once

#include "render/common.hpp"
#include "tg_types.hpp"

namespace cookcookhnya::render::shopping_list {

void renderShoppingList(
    UserId userId, ChatId chatId, tg_types::MessageId messageId, BotRef bot, ShoppingListApiRef api);

} // namespace cookcookhnya::render::shopping_list
