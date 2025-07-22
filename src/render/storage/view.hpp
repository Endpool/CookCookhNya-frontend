#pragma once

#include "backend/api/storages.hpp"
#include "backend/id_types.hpp"
#include "render/common.hpp"

namespace cookcookhnya::render::storage {

void renderStorageView(
    api::StorageId storageId, UserId userId, ChatId chatId, BotRef bot, api::StorageApiRef storageApi);

} // namespace cookcookhnya::render::storage
