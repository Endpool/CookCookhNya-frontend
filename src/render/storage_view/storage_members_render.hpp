#pragma once

#include "render/types_for_render.hpp"

namespace cookcookhnya::render::viewStorageMembers {
using namespace cookcookhnya::render;

void renderMemberList(const StorageId& storageId, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi);

void renderMemberAdditionDeletionPrompt(
    const StorageId& storageId, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi);
} // namespace cookcookhnya::render::viewStorageMembers
