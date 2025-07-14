#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::main_menu {

void handleMainMenuCQ(MainMenu& /*unused*/, CallbackQueryRef cq, BotRef& bot, SMRef stateManager, ApiClientRef api);

} // namespace cookcookhnya::handlers::main_menu
