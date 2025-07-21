#pragma once

#include "backend/api/ingredients.hpp"
#include "render/common.hpp"

#include <string>

namespace cookcookhnya::render::personal_account::ingredients {

void renderCustomIngredientCreation(UserId userId, ChatId chatId, BotRef bot);

void renderCustomIngredientConfirmation(
    std::string ingredientName, UserId userId, ChatId chatId, BotRef bot, api::IngredientsApiRef api);

} // namespace cookcookhnya::render::personal_account::ingredients
