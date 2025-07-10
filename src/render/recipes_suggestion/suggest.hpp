#pragma once

#include "render/common.hpp"

namespace cookcookhnya::render::recipes_suggestion {
using namespace tg_types;
InlineKeyboard constructMarkup(size_t pageNo, size_t numOfRecipesOnPage, api::models::recipe::RecipesList& recipesList);
InlineKeyboard constuctNavigationsMarkup(size_t offset,
                                         size_t fullKeyBoardSize,
                                         size_t pageNo,
                                         size_t numOfRecipesOnPage,
                                         api::models::recipe::RecipesList recipesList);
void editRecipesSuggestion(const std::vector<api::StorageId>& storageIds,
                           size_t pageNo,
                           UserId userId,
                           ChatId chatId,
                           BotRef bot,
                           RecipesApiRef recipesApi);

} // namespace cookcookhnya::render::recipes_suggestion
