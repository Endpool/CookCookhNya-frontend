#pragma once

#include "render/common.hpp"

namespace cookcookhnya::render::custom_recipe_delete {
void renderCustomRecipesListDelete(int pageNo, UserId userId, ChatId chatId, BotRef bot, RecipesApiRef recipesApi);

InlineKeyboard constructMarkup(int pageNo, int numOfRecipesOnPage, api::models::recipe::CustomRecipesList recipesList);
} // namespace cookcookhnya::render::custom_recipe_delete
