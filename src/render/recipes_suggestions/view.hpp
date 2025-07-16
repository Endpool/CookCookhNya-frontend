#pragma once

#include "backend/id_types.hpp"
#include "backend/models/recipe.hpp"
#include "render/common.hpp"

#include <cstddef>
#include <vector>

namespace cookcookhnya::render::recipes_suggestions {

using namespace tg_types;

InlineKeyboard constructMarkup(size_t pageNo, size_t numOfRecipesOnPage, api::models::recipe::RecipesList& recipesList);

InlineKeyboard constructNavigationsMarkup(size_t offset,
                                          size_t fullKeyBoardSize,
                                          size_t pageNo,
                                          size_t numOfRecipesOnPage,
                                          api::models::recipe::RecipesList recipesList);

void renderRecipesSuggestion(const std::vector<api::StorageId>& storageIds,
                             size_t pageNo,
                             UserId userId,
                             ChatId chatId,
                             BotRef bot,
                             RecipesApiRef recipesApi);

} // namespace cookcookhnya::render::recipes_suggestions
