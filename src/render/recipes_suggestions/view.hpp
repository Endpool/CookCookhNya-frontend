#pragma once

#include "backend/api/recipes.hpp"
#include "backend/id_types.hpp"
#include "render/common.hpp"

#include <cstddef>
#include <vector>

namespace cookcookhnya::render::recipes_suggestions {

using namespace tg_types;

void renderRecipesSuggestion(const std::vector<api::StorageId>& storages,
                             std::size_t pageNo,
                             UserId userId,
                             ChatId chatId,
                             BotRef bot,
                             api::RecipesApiRef recipesApi);

} // namespace cookcookhnya::render::recipes_suggestions
