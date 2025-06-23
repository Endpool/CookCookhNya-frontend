#pragma once

#include "handlers/type_refs.hpp"

namespace cookcookhnya::handlers::storageView {
using namespace cookcookhnya::forHandlers;

void storageViewButtonCallback(
    StorageView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi);
} // namespace cookcookhnya::handlers::storageView
