#pragma once

#include "render/common.hpp"
#include <cstddef>

namespace cookcookhnya::render::custom_recipes_list {

void renderCustomRecipesList(int pageNo, UserId userId, ChatId chatId, BotRef bot, RecipesApiRef recipesApi);

InlineKeyboard constructMarkup(int pageNo, int numOfRecipesOnPage, api::models::recipe::CustomRecipesList& recipesList);
InlineKeyboard constuctNavigationsMarkup(std::size_t offset,
                                         std::size_t fullKeyBoardSize,
                                         int pageNo,
                                         int numOfRecipesOnPage,
                                         api::models::recipe::CustomRecipesList recipesList);
} // namespace cookcookhnya::render::custom_recipes_list
