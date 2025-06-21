#pragma once

#include "handlers/type_refs.hpp"

namespace cookcookhnya::handlers::storageView {
using namespace cookcookhnya::forHandlers;

void storageViewButtonCallback(
    StorageView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, BackendApiRef api);
} // namespace cookcookhnya::handlers::storageView
