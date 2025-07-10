#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::init {

void start(MessageRef m, BotRef bot, SMRef stateManager, ApiClientRef api);

void handleNoState(MessageRef m, BotRef bot);

} // namespace cookcookhnya::handlers::init
