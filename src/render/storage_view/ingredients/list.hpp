#pragma once

#include "render/common.hpp"
#include "tg_types.hpp"

namespace cookcookhnya::render::storage::ingredients {

void renderIngredientsList(
    api::StorageId storage, UserId user, ChatId chat, tg_types::MessageId messageId, BotRef bot, IngredientsApiRef api);

} // namespace cookcookhnya::render::storage::ingredients
