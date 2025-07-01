#pragma once

#include "render/common.hpp"

namespace cookcookhnya::render::storage::member_list {

void renderMemberList(
    bool toBeEdited, const StorageId& storageId, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi);

void renderMemberAdditionPrompt(
    const StorageId& storageId, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi);

void renderMemberDeletionPrompt(
    const StorageId& storageId, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi);

} // namespace cookcookhnya::render::storage::member_list
