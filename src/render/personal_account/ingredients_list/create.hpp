#pragma once

#include "render/common.hpp"

namespace cookcookhnya::render::personal_account::ingredients {

void renderCustomIngredientCreation(UserId userId, ChatId chatId, BotRef bot);

void renderCustomIngredientConfirmation(
    std::string ingredientName, UserId userId, ChatId chatId, BotRef bot, IngredientsApiRef api);

} // namespace cookcookhnya::render::personal_account::ingredients
