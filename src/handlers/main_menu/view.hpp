#pragma once

#include "backend/api/api.hpp"
#include "handlers/common.hpp"

namespace cookcookhnya::handlers::main_menu {

void handleMainMenuCQ(MainMenu&, CallbackQueryRef cq, BotRef& bot, SMRef stateManager, api::ApiClientRef api);

} // namespace cookcookhnya::handlers::main_menu
