#pragma once

#include "../typeRefs.hpp"

namespace handlers::storageView {
using namespace cookcookhnya::forHandlers;

extern void storageViewButtonCallback(StorageView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, BackendApiRef api);
using storageViewButtonHandler = Handler<Events::CallbackQuery{}, storageViewButtonCallback>;
} // namespace handlers::storageView
