#pragma once

#include "backend/models/recipe.hpp"
#include "render/common.hpp"

namespace cookcookhnya::render::personal_account::recipe {

void renderPublicationHistory(UserId userId,
                              ChatId chatId,
                              std::string& recipeName,
                              std::string& errorReport,
                              std::vector<api::models::recipe::RecipePublicationRequest> history,
                              BotRef bot);
void renderPublicationRules(UserId userId, ChatId chatId, BotRef bot);
} // namespace cookcookhnya::render::personal_account::recipe
