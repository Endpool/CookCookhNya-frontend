#pragma once

#include "backend/models/recipe.hpp"
#include "render/common.hpp"

namespace cookcookhnya::render::recipe {

void renderRecipeView(const api::models::recipe::RecipeDetails& recipe, UserId userId, ChatId chatId, BotRef bot);

} // namespace cookcookhnya::render::recipe
