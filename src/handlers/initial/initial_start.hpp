#pragma once

#include "handlers/type_refs.hpp"

namespace cookcookhnya::handlers::init {

using namespace cookcookhnya::forHandlers;

void start(MessageRef m, BotRef bot, SMRef stateManager, UserApiRef userApi, StorageApiRef storageApi);

void handleNoState(MessageRef m, BotRef bot);

} // namespace cookcookhnya::handlers::init
