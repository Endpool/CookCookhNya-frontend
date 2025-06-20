#pragma once

#include "../typeForRender.hpp"

namespace render::viewStorageMembers{
using namespace cookcookhnya::render;

extern void renderMemberList(const StorageId& storageId, UserId userId, ChatId chatId, BotRef bot, BackendApiRef api);

extern void renderMemberAdditionDeletionPrompt(const StorageId& storageId, UserId userId, ChatId chatId, BotRef bot, BackendApiRef api);
} // namespace render::viewStorageMembers
