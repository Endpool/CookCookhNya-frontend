#pragma once

#include "render/common.hpp"
#include "states.hpp"
#include <cstddef>
namespace cookcookhnya::render::recipe::ingredients {

void renderRecipeIngredientsSearch(const states::CustomRecipeIngredientsSearch& state,
                                   size_t numOfIngredientsOnPage,
                                   UserId userId,
                                   ChatId chatId,
                                   BotRef bot);

} // namespace cookcookhnya::render::recipe::ingredients
