#pragma once

#include "backend/id_types.hpp"
#include "render/common.hpp"

namespace cookcookhnya::render::custom_recipes_list {

void renderCustomRecipesList(int pageNo, UserId userId, ChatId chatId, BotRef bot, RecipesApiRef recipesApi);

InlineKeyboard constructMarkup(int pageNo, int numOfRecipesOnPage, api::models::recipe::CustomRecipesList& recipesList);
InlineKeyboard constuctNavigationsMarkup(size_t offset,
                                         size_t fullKeyBoardSize,
                                         int pageNo,
                                         int numOfRecipesOnPage,
                                         api::models::recipe::CustomRecipesList recipesList);
} // namespace cookcookhnya::render::custom_recipes_list
