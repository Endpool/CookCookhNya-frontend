#pragma once

#include "backend/api/recipes.hpp"
#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"
#include "render/common.hpp"

#include <vector>

namespace cookcookhnya::render::personal_account::recipes {

std::vector<api::models::ingredient::Ingredient> renderCustomRecipe(
    bool toBeEdited, UserId userId, ChatId chatId, api::RecipeId recipeId, BotRef bot, api::RecipesApiRef recipesApi);

} // namespace cookcookhnya::render::personal_account::recipes
