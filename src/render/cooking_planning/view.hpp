#pragma once

#include "backend/api/api.hpp"
#include "backend/id_types.hpp"
#include "render/common.hpp"
#include "states.hpp"

#include <vector>

namespace cookcookhnya::render::cooking_planning {

void renderCookingPlanning(std::vector<states::CookingPlanning::IngredientAvailability>& inStoragesAvailability,
                           api::RecipeId recipeId,
                           UserId userId,
                           ChatId chatId,
                           BotRef bot,
                           api::ApiClientRef api);

} // namespace cookcookhnya::render::cooking_planning
