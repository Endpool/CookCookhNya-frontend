#pragma once

#include "tg_types.hpp"

#include <optional>

namespace cookcookhnya::message {

std::optional<tg_types::MessageId> getMessageId(tg_types::UserId userId);

void addMessageId(tg_types::UserId userId, tg_types::MessageId messageId);

} // namespace cookcookhnya::message
