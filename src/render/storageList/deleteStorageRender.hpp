#pragma once

#include "../typeForRender.hpp"

namespace cookcookhnya::render::deleteStorage {
using namespace cookcookhnya::render;

extern void renderStorageDelete(ChatId chatId, BotRef bot, UserId userId,  BackendApiRef api);

} // namespace cookcookhnya::render::deleteStorage
