#pragma once

#include "render/common.hpp"

namespace cookcookhnya::render::storage::ingredients {

void renderIngredientsList(api::StorageId storage, UserId user, ChatId chat, BotRef bot, IngredientsApiRef api);

} // namespace cookcookhnya::render::storage::ingredients
