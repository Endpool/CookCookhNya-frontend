#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::history {

void handleAllPublicationHistoryCQ(
    AllPublicationHistory& /*unused*/, CallbackQueryRef cq, BotRef& bot, SMRef stateManager, ApiClientRef api);

} // namespace cookcookhnya::handlers::history
