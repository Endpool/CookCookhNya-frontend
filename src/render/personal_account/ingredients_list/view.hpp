#pragma once

#include "backend/api/ingredients.hpp"
#include "render/common.hpp"

namespace cookcookhnya::render::personal_account::ingredients {

void renderCustomIngredientsList(bool toBeEdited, UserId userId, ChatId chatId, BotRef bot, api::IngredientsApiRef api);

} // namespace cookcookhnya::render::personal_account::ingredients
