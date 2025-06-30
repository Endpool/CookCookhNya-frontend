#pragma once

#include "render/common.hpp"

namespace cookcookhnya::render::select_storages {

using namespace tg_types;
void renderStoragesSelect(
    std::vector<StorageId> selected_storages, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi);

void editStorageSelectMessage(
    std::vector<StorageId> selected_storages, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi);

} // namespace cookcookhnya::render::select_storages
