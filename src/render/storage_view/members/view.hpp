#pragma once

#include "backend/id_types.hpp"
#include "render/common.hpp"

namespace cookcookhnya::render::storage::member_list {

void renderMemberList(bool toBeEdited,
                      const api::StorageId& storageId,
                      UserId userId,
                      ChatId chatId,
                      BotRef bot,
                      StorageApiRef storageApi);

} // namespace cookcookhnya::render::storage::member_list
