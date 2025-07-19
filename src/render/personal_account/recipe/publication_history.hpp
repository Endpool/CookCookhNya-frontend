#pragma once

#include "backend/id_types.hpp"
#include "render/common.hpp"

namespace cookcookhnya::render::personal_account::publication_history {

void renderPublicationHistory(
    UserId userId, ChatId chatId, api::RecipeId recipeId, BotRef bot, RecipesApiRef recipesApi);

} // namespace cookcookhnya::render::personal_account::publication_history
