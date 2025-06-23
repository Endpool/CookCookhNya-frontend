#pragma once

#include "handlers/type_refs.hpp"

namespace cookcookhnya::handlers::storageListView {
using namespace cookcookhnya::forHandlers;

void storageListButtonCallback(
    StorageList&, CallbackQueryRef cq, const Api& bot, SMRef stateManager, StorageApiRef storageApi);

} // namespace cookcookhnya::handlers::storageListView
