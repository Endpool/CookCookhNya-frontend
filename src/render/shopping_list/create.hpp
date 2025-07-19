#pragma once

#include "backend/models/ingredient.hpp"
#include "render/common.hpp"

#include <vector>

namespace cookcookhnya::render::shopping_list {

void renderShoppingListCreation(const std::vector<api::models::ingredient::Ingredient>& selectedIngredients,
                                const std::vector<api::models::ingredient::Ingredient>& allIngredients,
                                UserId userId,
                                ChatId chatId,
                                BotRef bot);

} // namespace cookcookhnya::render::shopping_list
