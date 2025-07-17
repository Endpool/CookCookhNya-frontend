#pragma once

#include "backend/api/ingredients.hpp"

#include "backend/id_types.hpp"
#include "render/common.hpp"

#include <vector>

namespace cookcookhnya::render::shopping_list {

void renderShoppingListCreation(std::vector<api::models::ingredient::Ingredient> selectedIngredients,
                                UserId userId,
                                ChatId chatId,
                                BotRef bot);

} // namespace cookcookhnya::render::shopping_list
