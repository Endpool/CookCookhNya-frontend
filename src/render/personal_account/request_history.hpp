#pragma once

#include "render/common.hpp"

namespace cookcookhnya::render::personal_account::publication_history {
void renderRequestHistory(
    UserId userId, size_t pageNo, size_t numOfInstances, ChatId chatId, BotRef bot, RequestHistoryApiRef reqHistoryApi);
} // namespace cookcookhnya::render::personal_account::publication_history
