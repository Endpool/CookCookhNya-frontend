#pragma once

#include "backend/api/storages.hpp"
#include "render/common.hpp"

#include <optional>
#include <string>

namespace cookcookhnya::render::main_menu {

void renderMainMenu(bool toBeEdited,
                    std::optional<std::optional<std::string>> inviteStorage,
                    UserId userId,
                    ChatId chatId,
                    BotRef bot,
                    api::StorageApiRef storageApi);

} // namespace cookcookhnya::render::main_menu
