#pragma once

#include "backend/api/ingredients.hpp"

#include "backend/id_types.hpp"
#include "render/common.hpp"

#include <vector>

namespace cookcookhnya::render::shopping_list {

std::vector<api::IngredientId> renderShoppingListCreation(const std::vector<api::IngredientId>& ingredientIds,
                                                          UserId userId,
                                                          ChatId chatId,
                                                          BotRef bot,
                                                          api::IngredientsApi ingredientsApi);

} // namespace cookcookhnya::render::shopping_list
