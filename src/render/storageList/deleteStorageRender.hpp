#pragma once

#include "render/typeForRender.hpp"

namespace cookcookhnya::render::deleteStorage {
using namespace cookcookhnya::render;

void renderStorageDelete(ChatId chatId, BotRef bot, UserId userId,  BackendApiRef api);

} // namespace cookcookhnya::render::deleteStorage
