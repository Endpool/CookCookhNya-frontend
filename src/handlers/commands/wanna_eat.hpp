#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::commands {

void handleWannaEatCmd(MessageRef m, BotRef bot, SMRef stateManager, ApiClientRef api);

} // namespace cookcookhnya::handlers::commands
