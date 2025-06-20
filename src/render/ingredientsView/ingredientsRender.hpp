#pragma once

#include "../typeForRender.hpp"

namespace render::viewIngredients{
using namespace cookcookhnya::render;

extern void renderIngredientsList(StorageId storageId, UserId userId, ChatId chatId, BotRef bot);
} // namespace render::viewIngredients
