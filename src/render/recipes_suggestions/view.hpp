#pragma once

#include "backend/models/storage.hpp"
#include "render/common.hpp"

#include <cstddef>
#include <vector>

namespace cookcookhnya::render::recipes_suggestions {

using namespace tg_types;

void renderRecipesSuggestion(std::vector<api::models::storage::StorageSummary>& storages,
                             std::size_t pageNo,
                             UserId userId,
                             ChatId chatId,
                             BotRef bot,
                             RecipesApiRef recipesApi);

} // namespace cookcookhnya::render::recipes_suggestions
