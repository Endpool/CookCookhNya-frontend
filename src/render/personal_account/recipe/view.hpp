#pragma once

#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"
#include "render/common.hpp"
#include <vector>

namespace cookcookhnya::render::personal_account::recipes {

struct IngredientsAndRecipeName {
    std::vector<api::models::ingredient::Ingredient> ingredients;
    std::string recipeName;
};

IngredientsAndRecipeName renderCustomRecipe(
    bool toBeEdited, UserId userId, ChatId chatId, api::RecipeId recipeId, BotRef bot, RecipesApiRef recipesApi);

} // namespace cookcookhnya::render::personal_account::recipes
