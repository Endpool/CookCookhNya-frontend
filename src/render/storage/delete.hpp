#pragma once

#include "backend/api/storages.hpp"
#include "backend/id_types.hpp"
#include "render/common.hpp"

namespace cookcookhnya::render::delete_storage {

void renderStorageDeletion(
    api::StorageId storageId, ChatId chatId, BotRef bot, UserId userId, api::StorageApiRef storageApi);

} // namespace cookcookhnya::render::delete_storage
