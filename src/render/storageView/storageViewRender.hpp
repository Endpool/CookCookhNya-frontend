#pragma once

#include "../typeForRender.hpp"

namespace render::viewStorage{
using namespace cookcookhnya::render;

extern void renderStorageView(StorageId storageId, UserId userId, ChatId chatId, BotRef bot, BackendApiRef api);
} // namespace render::viewStorage
