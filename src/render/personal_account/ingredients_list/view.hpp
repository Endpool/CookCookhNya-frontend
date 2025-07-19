#pragma once

#include "render/common.hpp"

namespace cookcookhnya::render::personal_account::ingredients {

void renderCustomIngredientsList(
    bool toBeEdited, std::size_t pageNo, UserId userId, ChatId chatId, BotRef bot, IngredientsApiRef api);

} // namespace cookcookhnya::render::personal_account::ingredients
