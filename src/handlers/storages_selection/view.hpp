#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::storages_selection {

void handleStoragesSelectionCQ(
    StoragesSelection& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api);

} // namespace cookcookhnya::handlers::storages_selection
