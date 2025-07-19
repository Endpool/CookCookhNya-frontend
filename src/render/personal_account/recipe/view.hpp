#pragma once

#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"
#include "render/common.hpp"
#include <vector>

namespace cookcookhnya::render::personal_account::recipes {

std::tuple<std::vector<api::models::ingredient::Ingredient>, std::string> renderCustomRecipe(
    bool toBeEdited, UserId userId, ChatId chatId, api::RecipeId recipeId, BotRef bot, RecipesApiRef recipesApi);

} // namespace cookcookhnya::render::personal_account::recipes
