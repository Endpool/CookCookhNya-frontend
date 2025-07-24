#pragma once

#include "render/common.hpp"
#include "states.hpp"

#include <vector>

namespace cookcookhnya::render::cooking {

void renderIngredientsSpending(const std::vector<states::helpers::SelectableIngredient>& ingredients,
                               bool canRemove,
                               UserId userId,
                               ChatId chatId,
                               BotRef bot);

} // namespace cookcookhnya::render::cooking
