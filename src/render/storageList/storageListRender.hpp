#pragma once

#include "../typeForRender.hpp"

namespace render::storageList{
using namespace cookcookhnya::render;

extern void renderStorageList(UserId userId, ChatId chatId, BotRef bot, BackendApiRef api);
} // namespace render::storageList
