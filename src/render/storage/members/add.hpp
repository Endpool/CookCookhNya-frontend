#pragma once

#include "backend/id_types.hpp"
#include "render/common.hpp"

namespace cookcookhnya::render::storage::members {

void renderStorageMemberAddition(
    const api::StorageId& storageId, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi);

void renderShareLinkMemberAddition(
    const api::StorageId& storageId, UserId userId, ChatId chatId, BotRef bot, StorageApiRef storageApi);

} // namespace cookcookhnya::render::storage::members
