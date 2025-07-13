#pragma once

#include "render/common.hpp"

namespace cookcookhnya::render::personal_account::ingredients {

void renderCustomIngredientCreate(UserId userId, ChatId chatId, BotRef bot);

void renderCustomIngredientConfirm(
    std::string ingredientName, UserId userId, ChatId chatId, BotRef bot, IngredientsApiRef api);

} // namespace cookcookhnya::render::personal_account::ingredients
