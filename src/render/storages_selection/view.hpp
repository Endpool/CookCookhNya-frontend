#pragma once

#include "backend/api/storages.hpp"
#include "render/common.hpp"
#include "states.hpp"

namespace cookcookhnya::render::select_storages {

void renderStorageSelection(
    const states::StoragesSelection& state, UserId userId, ChatId chatId, BotRef bot, api::StorageApiRef storageApi);

} // namespace cookcookhnya::render::select_storages
