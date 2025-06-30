#pragma once

#include "render/common.hpp"

namespace cookcookhnya::render::storage::ingredients {

void renderIngredientsList(StorageId storage, UserId userId, ChatId chat, BotRef bot, IngredientsApiRef api);

} // namespace cookcookhnya::render::storage::ingredients
