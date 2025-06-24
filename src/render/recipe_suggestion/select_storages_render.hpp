#pragma once

#include "render/common.hpp"
#include "tg_types.hpp"

namespace cookcookhnya::render::select_storages {

MessageId renderStoragesSelect(UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi);

void updateStorageSelect(std::vector<StorageId> selected_storages, MessageId messageId, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi);

} // namespace cookcookhnya::render::select_storages
