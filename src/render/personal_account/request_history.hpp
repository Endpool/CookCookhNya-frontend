#pragma once

#include "render/common.hpp"

namespace cookcookhnya::render::personal_account::history {
void renderRequestHistory(
    UserId userId, size_t pageNo, size_t numOfInstances, ChatId chatId, BotRef bot, RecipesApiRef recipeApi);
} // namespace cookcookhnya::render::personal_account::history
