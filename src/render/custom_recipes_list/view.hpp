#pragma once

#include "render/common.hpp"
#include <cstddef>

namespace cookcookhnya::render::custom_recipes_list {

void renderCustomRecipesList(size_t pageNo, UserId userId, ChatId chatId, BotRef bot, RecipesApiRef recipesApi);

InlineKeyboard
constructMarkup(size_t pageNo, size_t numOfRecipesOnPage, api::models::recipe::CustomRecipesList& recipesList);
InlineKeyboard constuctNavigationsMarkup(size_t offset,
                                         size_t fullKeyBoardSize,
                                         size_t pageNo,
                                         size_t numOfRecipesOnPage,
                                         api::models::recipe::CustomRecipesList recipesList);
} // namespace cookcookhnya::render::custom_recipes_list
