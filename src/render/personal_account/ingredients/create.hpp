#pragma once

#include "render/common.hpp"

namespace cookcookhnya::render::create_custom_ingredient {

void renderCustomIngredientCreate(UserId userId, ChatId chatId, BotRef bot);

void renderCustomIngredientConfirm(std::string name, UserId userId, ChatId chatId, BotRef bot, IngredientsApiRef api);

} // namespace cookcookhnya::render::create_custom_ingredient
