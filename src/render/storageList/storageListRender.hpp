#pragma once

#include "../typeForRender.hpp"

namespace cookcookhnya::render::storageList{
using namespace cookcookhnya::render;

void renderStorageList(UserId userId, ChatId chatId, BotRef bot, BackendApiRef api);
} // namespace cookcookhnya::render::storageList
