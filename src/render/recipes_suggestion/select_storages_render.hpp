#pragma once

#include "backend/id_types.hpp"
#include "render/common.hpp"

#include <vector>

namespace cookcookhnya::render::select_storages {

void renderStorageSelect(std::vector<api::StorageId> const& selected_storages,
                         UserId userId,
                         ChatId chatId,
                         BotRef bot,
                         StorageApiRef storageApi);

void editStorageSelect(std::vector<api::StorageId> const& selected_storages,
                       UserId userId,
                       ChatId chatId,
                       BotRef bot,
                       StorageApiRef storageApi);

} // namespace cookcookhnya::render::select_storages
