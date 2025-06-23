#pragma once

#include "render/types_for_render.hpp"

namespace cookcookhnya::render::deleteStorage {
using namespace cookcookhnya::render;

void renderStorageDelete(ChatId chatId, BotRef bot, UserId userId, StorageApiRef storageApi);

} // namespace cookcookhnya::render::deleteStorage
