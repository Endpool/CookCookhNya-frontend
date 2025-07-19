#pragma once

#include "render/common.hpp"
#include "states.hpp"

namespace cookcookhnya::render::select_storages {

void renderStorageSelection(
    const states::StoragesSelection& state, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi);

} // namespace cookcookhnya::render::select_storages
