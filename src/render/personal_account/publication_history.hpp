#pragma once

#include "backend/api/moderation.hpp"
#include "render/common.hpp"

namespace cookcookhnya::render::personal_account {

void renderRequestHistory(UserId userId,
                          std::size_t pageNo,
                          std::size_t numOfInstances,
                          ChatId chatId,
                          BotRef bot,
                          api::ModerationApiRef moderationApi);

} // namespace cookcookhnya::render::personal_account
