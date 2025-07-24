#pragma once

#include "backend/api/recipes.hpp"
#include "render/common.hpp"

#include <cstddef>

namespace cookcookhnya::render::personal_account::recipes_list {

void renderCustomRecipesList(
    std::size_t pageNo, UserId userId, ChatId chatId, BotRef bot, api::RecipesApiRef recipesApi);

} // namespace cookcookhnya::render::personal_account::recipes_list
