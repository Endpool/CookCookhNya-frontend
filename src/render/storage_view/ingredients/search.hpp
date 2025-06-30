#pragma once

#include "render/common.hpp"
#include "tg_types.hpp"

namespace cookcookhnya::render::storage::ingredients {

tg_types::MessageId renderStorageIngredientsSearchSend(ChatId chatId, BotRef bot, IngredientsApiRef api);

void renderStorageIngredientsSearchEdit(tg_types::MessageId message, ChatId chatId, BotRef bot, IngredientsApiRef api);

} // namespace cookcookhnya::render::storage::ingredients
