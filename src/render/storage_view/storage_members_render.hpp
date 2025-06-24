#pragma once

#include "render/common.hpp"

namespace cookcookhnya::render::view_storage_members {

void renderMemberList(const StorageId& storageId, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi);

void renderMemberAdditionDeletionPrompt(
    const StorageId& storageId, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi);

} // namespace cookcookhnya::render::view_storage_members
