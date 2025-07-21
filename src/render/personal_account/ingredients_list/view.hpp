#pragma once

#include "backend/api/ingredients.hpp"
#include "render/common.hpp"

#include <cstddef>

namespace cookcookhnya::render::personal_account::ingredients {

void renderCustomIngredientsList(
    bool toBeEdited, std::size_t pageNo, UserId userId, ChatId chatId, BotRef bot, api::IngredientsApiRef api);

} // namespace cookcookhnya::render::personal_account::ingredients
