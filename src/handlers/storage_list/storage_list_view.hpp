#pragma once

#include "handlers/type_refs.hpp"

namespace cookcookhnya::handlers::storageListView {
using namespace cookcookhnya::handlers;

void storageListButtonCallback(
    StorageList&, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi);

} // namespace cookcookhnya::handlers::storageListView
