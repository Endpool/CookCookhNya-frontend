#pragma once

#include "../typeRefs.hpp"

namespace handlers::init {
using namespace cookcookhnya::forHandlers;

extern void start(MessageRef m, BotRef bot, SMRef stateManager, BackendApiRef api);

extern void handleNoState(MessageRef m, BotRef bot);
using noStateHandler = Handler<Events::AnyMessage{}, handleNoState, HandlerTypes::NoState{}>;
}