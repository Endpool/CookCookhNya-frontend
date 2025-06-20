#pragma once

#include "render/types_for_render.hpp"

namespace cookcookhnya::render::viewStorage{
using namespace cookcookhnya::render;

void renderStorageView(StorageId storageId, UserId userId, ChatId chatId, BotRef bot, BackendApiRef api);
} // namespace cookcookhnya::render::viewStorage
