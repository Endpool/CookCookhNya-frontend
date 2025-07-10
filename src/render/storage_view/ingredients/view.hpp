#pragma once

#include "render/common.hpp"
#include "states.hpp"

namespace cookcookhnya::render::storage::ingredients {

void renderIngredientsListSearch(const states::StorageIngredientsList& state, UserId user, ChatId chat, BotRef bot);

} // namespace cookcookhnya::render::storage::ingredients
