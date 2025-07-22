#pragma once

#include "render/common.hpp"
#include "states.hpp"

namespace cookcookhnya::render::storage::ingredients {

void renderStorageIngredientsDeletion(states::StorageIngredientsDeletion& state,
                                      UserId userId,
                                      ChatId chatId,
                                      BotRef bot);

} // namespace cookcookhnya::render::storage::ingredients
