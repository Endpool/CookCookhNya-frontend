#pragma once

#include "backend/api/ingredients.hpp"
#include "backend/id_types.hpp"
#include "render/common.hpp"

namespace cookcookhnya::render::shopping_list_creation {

std::vector<api::IngredientId> renderShoppingListCreation( // It returns ingredient Ids which are on the list
    const std::vector<api::StorageId>& storageIds,
    api::RecipeId recipeId,
    UserId userId,
    ChatId chatId,
    BotRef bot,
    ApiClient api);

void renderEditedShoppingListCreation(std::vector<api::IngredientId> ingredientIds,
                                      api::IngredientId ingredientIdToRemove,
                                      ChatId chatId,
                                      tg_types::MessageId messageId,
                                      BotRef bot,
                                      IngredientsApiRef ingredientsApi);

} // namespace cookcookhnya::render::shopping_list_creation
