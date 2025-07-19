#pragma once

#include "backend/id_types.hpp"
#include "render/common.hpp"

namespace cookcookhnya::render::delete_storage {

void renderStorageDeletion(
    api::StorageId storageId, ChatId chatId, BotRef bot, UserId userId, StorageApiRef storageApi);

} // namespace cookcookhnya::render::delete_storage
