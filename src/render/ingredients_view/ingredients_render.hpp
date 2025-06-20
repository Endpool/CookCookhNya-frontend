#pragma once

#include "render/types_for_render.hpp"

namespace cookcookhnya::render::viewIngredients{
using namespace cookcookhnya::render;

void renderIngredientsList(StorageId storageId, UserId userId, ChatId chatId, BotRef bot);
} // namespace cookcookhnya::render::viewIngredients
