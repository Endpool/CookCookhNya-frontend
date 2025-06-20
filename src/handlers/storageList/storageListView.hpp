#pragma once

#include "../typeRefs.hpp"

namespace handlers::storageListView {
using namespace cookcookhnya::forHandlers;

extern void storageListButtonCallback(StorageList&,
                                      CallbackQueryRef cq,
                                      const Api& bot,
                                      SMRef stateManager, BackendApiRef api);
                                   
} // namespace handlers::storageListView
