#pragma once

#include "render/common.hpp"
#include "states.hpp"

namespace cookcookhnya::render::recipes_search {

void renderRecipesSearch(const states::helpers::Pagination& pagination,
                         const decltype(states::RecipesSearch::page)& page,
                         UserId userId,
                         ChatId chatId,
                         BotRef bot);

} // namespace cookcookhnya::render::recipes_search
