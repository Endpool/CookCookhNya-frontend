#pragma once

#include "backend/api/recipes.hpp"
#include "backend/id_types.hpp"
#include "render/common.hpp"

namespace cookcookhnya::render::personal_account::recipe {

void renderPublicationHistory(UserId userId,
                              ChatId chatId,
                              api::RecipeId recipeId,
                              std::string& recipeName,
                              bool isPeek,
                              BotRef bot,
                              api::RecipesApiRef recipesApi);

} // namespace cookcookhnya::render::personal_account::recipe
