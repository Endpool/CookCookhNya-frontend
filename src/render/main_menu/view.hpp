#pragma once

#include "render/common.hpp"

#include <optional>

namespace cookcookhnya::render::main_menu {

void renderMainMenu(bool toBeEdited,
                    std::optional<std::optional<std::string>> inviteStorage,
                    UserId userId,
                    ChatId chatId,
                    BotRef bot,
                    StorageApiRef storageApi);

} // namespace cookcookhnya::render::main_menu
