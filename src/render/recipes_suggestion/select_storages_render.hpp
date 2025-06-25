#pragma once

#include "render/common.hpp"
#include "tg_types.hpp"

namespace cookcookhnya::render::select_storages {

tg_types::MessageId renderStoragesSelect(
    std::vector<StorageId> selected_storages, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi);

void updateStorageSelect(std::vector<StorageId> selected_storages,
                         tg_types::MessageId messageId,
                         UserId userId,
                         ChatId chatId,
                         BotRef bot,
                         StorageApiRef storageApi);

} // namespace cookcookhnya::render::select_storages
