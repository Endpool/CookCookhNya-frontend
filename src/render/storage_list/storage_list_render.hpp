#pragma once

#include "render/types_for_render.hpp"

namespace cookcookhnya::render::storageList {
using namespace cookcookhnya::render;

void renderStorageList(UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi);
} // namespace cookcookhnya::render::storageList
