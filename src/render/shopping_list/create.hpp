#pragma once

#include "render/common.hpp"

#include <vector>

namespace cookcookhnya::render::shopping_list {

void renderShoppingListCreation(std::vector<api::models::ingredient::Ingredient> selectedIngredients,
                                UserId userId,
                                ChatId chatId,
                                BotRef bot);

} // namespace cookcookhnya::render::shopping_list
