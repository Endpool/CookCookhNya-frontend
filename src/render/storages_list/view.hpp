#pragma once

#include "backend/api/storages.hpp"
#include "render/common.hpp"

namespace cookcookhnya::render::storages_list {

void renderStorageList(bool toBeEdited, UserId userId, ChatId chatId, BotRef bot, api::StorageApiRef storageApi);

} // namespace cookcookhnya::render::storages_list
