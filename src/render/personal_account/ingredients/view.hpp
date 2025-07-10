#pragma once

#include "render/common.hpp"

namespace cookcookhnya::render::view_custom_ingredients {

void renderCustomIngredientsList(bool toBeEdited, UserId userId, ChatId chatId, BotRef bot, IngredientsApiRef api);

} // namespace cookcookhnya::render::view_custom_ingredients
