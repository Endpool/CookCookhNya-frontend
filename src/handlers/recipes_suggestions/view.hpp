#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::recipes_suggestions {

enum struct AvailabiltiyType : std::uint8_t { available, not_available, other_storages };

struct IngredientAvailability { 
    AvailabiltiyType available = AvailabiltiyType::not_available;
    std::vector<api::models::storage::StorageSummary> storages;
};

void handleSuggestedRecipeListCQ(
    SuggestedRecipeList& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api);

} // namespace cookcookhnya::handlers::recipes_suggestions
