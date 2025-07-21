#pragma once

#include "render/common.hpp"
#include "states.hpp"

#include <cstddef>

namespace cookcookhnya::render::shopping_list {

void renderShoppingListIngredientSearch(
    const states::ShoppingListIngredientSearch& state, std::size_t pageSize, UserId userId, ChatId chatId, BotRef bot);

} // namespace cookcookhnya::render::shopping_list
