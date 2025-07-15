#pragma once

#include "backend/id_types.hpp"
#include "render/common.hpp"

namespace cookcookhnya::render::shopping_list {

std::vector<api::IngredientId> renderShoppingListCreation(const std::vector<api::IngredientId>& ingredientIds,
                                                          std::vector<std::string> ingredientsName,
                                                          UserId userId,
                                                          ChatId chatId,
                                                          BotRef bot);

} // namespace cookcookhnya::render::shopping_list
