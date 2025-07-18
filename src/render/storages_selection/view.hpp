#pragma once

#include "render/common.hpp"

#include <vector>

namespace cookcookhnya::render::select_storages {

void renderStorageSelection(const std::vector<api::models::storage::StorageSummary>& selectedStorages,
                            UserId userId,
                            ChatId chatId,
                            BotRef bot,
                            StorageApiRef storageApi);

} // namespace cookcookhnya::render::select_storages
