#pragma once

#include "render/common.hpp"

namespace cookcookhnya::render::storage {

void renderStorageView(api::StorageId storageId, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi);

} // namespace cookcookhnya::render::storage
