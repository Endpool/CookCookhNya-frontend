#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::commands {

void handleMyStoragesCmd(MessageRef m, BotRef bot, SMRef stateManager, ApiClientRef api);

} // namespace cookcookhnya::handlers::commands
