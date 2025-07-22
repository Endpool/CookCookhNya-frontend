#pragma once

#include "render/common.hpp"
#include "states.hpp"

namespace cookcookhnya::render::storage::ingredients {

void renderIngredientsListSearch(const states::StorageIngredientsList& state, UserId userId, ChatId chatId, BotRef bot);

} // namespace cookcookhnya::render::storage::ingredients

namespace cookcookhnya::render::suggest_custom_ingredient {

void renderSuggestIngredientCustomisation(const states::StorageIngredientsList& state,
                                          UserId userId,
                                          ChatId chatId,
                                          BotRef bot);

} // namespace cookcookhnya::render::suggest_custom_ingredient
