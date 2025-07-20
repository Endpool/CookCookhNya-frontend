#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::personal_account::recipe::publication_history {

void handleCustomRecipePublicationHistoryCQ(
    CustomRecipePublicationHistory& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api);

} // namespace cookcookhnya::handlers::personal_account::recipe::publication_history
