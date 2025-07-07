#pragma once

#include "render/common.hpp"

namespace cookcookhnya::render::main_menu {

void renderMainMenu(bool toBeEdited, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi);

} // namespace cookcookhnya::render::main_menu
