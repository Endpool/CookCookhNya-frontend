#pragma once

#include "render/common.hpp"

#include <cstddef>

namespace cookcookhnya::render::personal_account::recipes {

void renderCustomRecipesList(std::size_t pageNo, UserId userId, ChatId chatId, BotRef bot, RecipesApiRef recipesApi);

} // namespace cookcookhnya::render::personal_account::recipes
