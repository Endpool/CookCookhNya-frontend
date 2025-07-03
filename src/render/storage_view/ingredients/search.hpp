#pragma once

#include "render/common.hpp"
#include "tg_types.hpp"

#include <cstddef>

namespace cookcookhnya::render::storage::ingredients {

tg_types::MessageId renderStorageIngredientsSearchSend(ChatId chat, BotRef bot);

void renderStorageIngredientsSearchEdit(const std::vector<api::models::ingredient::IngredientSearchItem>&,
                                        std::size_t pageNo,
                                        std::size_t totalPages,
                                        tg_types::MessageId messageId,
                                        ChatId chat,
                                        BotRef bot);

} // namespace cookcookhnya::render::storage::ingredients
