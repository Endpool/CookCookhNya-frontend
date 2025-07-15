#pragma once

#include "backend/id_types.hpp"
#include "render/common.hpp"

#include <vector>

namespace cookcookhnya::render::shopping_list {

std::vector<api::IngredientId> renderShoppingListCreation( // It returns ingredient Ids which are on the list
    const std::vector<api::StorageId>& storageIds,
    api::RecipeId recipeId,
    UserId userId,
    ChatId chatId,
    BotRef bot,
    RecipesApiRef api);

void renderEditedShoppingListCreation(const std::vector<api::IngredientId>& ingredientIds,
                                      UserId userId,
                                      ChatId chatId,
                                      BotRef bot,
                                      IngredientsApiRef ingredientsApi);

} // namespace cookcookhnya::render::shopping_list
