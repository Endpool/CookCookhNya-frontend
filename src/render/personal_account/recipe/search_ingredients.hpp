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

namespace cookcookhnya::render::suggest_custom_ingredient {

void renderSuggestIngredientCustomisation(const states::CustomRecipeIngredientsSearch& state,
                                          UserId userId,
                                          ChatId chatId,
                                          BotRef bot);
} // namespace cookcookhnya::render::suggest_custom_ingredient
