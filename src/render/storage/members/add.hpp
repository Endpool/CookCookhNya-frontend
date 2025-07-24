#pragma once

#include "backend/api/storages.hpp"
#include "backend/id_types.hpp"
#include "render/common.hpp"

namespace cookcookhnya::render::storage::members {

void renderStorageMemberAddition(
    const api::StorageId& storageId, UserId userId, ChatId chatId, BotRef bot, api::StorageApiRef storageApi);

void renderShareLinkMemberAddition(
    const api::StorageId& storageId, UserId userId, ChatId chatId, BotRef bot, api::StorageApiRef storageApi);

} // namespace cookcookhnya::render::storage::members
