#pragma once

#include "backend/id_types.hpp"
#include "render/common.hpp"
#include "tg_types.hpp"

#include <vector>

namespace cookcookhnya::render::select_storages {

tg_types::MessageId renderStoragesSelect(const std::vector<api::StorageId>& selected_storages,
                                         UserId userId,
                                         ChatId chatId,
                                         BotRef bot,
                                         StorageApiRef storageApi);

void updateStorageSelect(const std::vector<api::StorageId>& selected_storages,
                         tg_types::MessageId messageId,
                         UserId userId,
                         ChatId chatId,
                         BotRef bot,
                         StorageApiRef storageApi);

} // namespace cookcookhnya::render::select_storages
