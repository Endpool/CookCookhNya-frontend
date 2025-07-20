#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::personal_account::publication_history {

void handleAllPublicationHistoryCQ(
    AllPublicationHistory& state, CallbackQueryRef cq, BotRef& bot, SMRef stateManager, ApiClientRef api);

} // namespace cookcookhnya::handlers::personal_account::publication_history
