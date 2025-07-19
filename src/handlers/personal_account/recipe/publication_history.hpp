#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::personal_account::publication_history {

void handleCustomRecipePublicationHistoryCQ(
    CustomRecipePublicationHistory& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, RecipesApiRef api);

} // namespace cookcookhnya::handlers::personal_account::publication_history
