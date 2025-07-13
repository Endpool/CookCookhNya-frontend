#pragma once

#include "backend/models/ingredient.hpp"

#include "render/common.hpp"
#include <cstddef>

namespace cookcookhnya::render::recipe::ingredients {

void renderRecipeIngredientsSearchEdit(
    const std::vector<api::models::ingredient::IngredientSearchForRecipeItem>& ingredients,
    std::size_t pageNo,
    std::size_t totalPages,
    MessageId message,
    ChatId chatId,
    BotRef bot);

void renderStorageIngredientsSearch(ChatId chatId, UserId userId, BotRef bot);
} // namespace cookcookhnya::render::recipe::ingredients
