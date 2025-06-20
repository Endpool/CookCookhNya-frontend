#pragma once

#include "../typeRefs.hpp"

namespace cookcookhnya::handlers::storageListView {
using namespace cookcookhnya::forHandlers;

void storageListButtonCallback(StorageList&,
                                      CallbackQueryRef cq,
                                      const Api& bot,
                                      SMRef stateManager, BackendApiRef api);
                                   
} // namespace cookcookhnya::handlers::storageListView
