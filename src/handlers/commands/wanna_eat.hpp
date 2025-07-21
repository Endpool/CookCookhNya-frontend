#pragma once

#include "backend/api/api.hpp"
#include "handlers/common.hpp"

namespace cookcookhnya::handlers::commands {

void handleWannaEatCmd(MessageRef m, BotRef bot, SMRef stateManager, api::ApiClientRef api);

} // namespace cookcookhnya::handlers::commands
