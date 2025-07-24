#pragma once

#include "render/common.hpp"
#include "states.hpp"

#include <cstddef>

namespace cookcookhnya::render::personal_account::recipe {

void renderRecipeIngredientsSearch(const states::CustomRecipeIngredientsSearch& state,
                                   size_t numOfIngredientsOnPage,
                                   UserId userId,
                                   ChatId chatId,
                                   BotRef bot);

void renderSuggestIngredientCustomisation(const states::CustomRecipeIngredientsSearch& state,
                                          UserId userId,
                                          ChatId chatId,
                                          BotRef bot);

} // namespace cookcookhnya::render::personal_account::recipe
