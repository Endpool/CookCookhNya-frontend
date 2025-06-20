#pragma once

#include "render/typeForRender.hpp"

namespace cookcookhnya::render::viewStorageMembers{
using namespace cookcookhnya::render;

void renderMemberList(const StorageId& storageId, UserId userId, ChatId chatId, BotRef bot, BackendApiRef api);

void renderMemberAdditionDeletionPrompt(const StorageId& storageId, UserId userId, ChatId chatId, BotRef bot, BackendApiRef api);
} // namespace cookcookhnya::render::viewStorageMembers
