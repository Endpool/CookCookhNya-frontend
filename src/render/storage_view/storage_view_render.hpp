#pragma once

#include "render/common.hpp"

namespace cookcookhnya::render::view_storage {

void renderStorageView(StorageId storageId, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi);

} // namespace cookcookhnya::render::view_storage
