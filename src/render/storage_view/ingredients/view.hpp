#pragma once

#include "render/common.hpp"
#include "tg_types.hpp"

#include <cstddef>

namespace cookcookhnya::render::storage::ingredients {

void renderIngredientsListSearch(api::StorageId storage, UserId user, ChatId chat, BotRef bot, IngredientsApiRef api);

void renderStorageIngredientsSearchEdit(const std::vector<api::models::ingredient::IngredientSearchItem>&,
                                        std::size_t pageNo,
                                        std::size_t totalPages,
                                        tg_types::MessageId messageId,
                                        ChatId chatId,
                                        BotRef bot);

} // namespace cookcookhnya::render::storage::ingredients
