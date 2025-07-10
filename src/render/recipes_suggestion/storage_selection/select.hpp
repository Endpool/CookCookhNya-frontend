#pragma once

#include "backend/id_types.hpp"
#include "render/common.hpp"

#include <vector>

namespace cookcookhnya::render::select_storages {

void renderStorageSelect(const std::vector<api::StorageId>& selected_storages,
                         UserId userId,
                         ChatId chatId,
                         BotRef bot,
                         StorageApiRef storageApi);

} // namespace cookcookhnya::render::select_storages
