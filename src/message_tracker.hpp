#pragma once

#include "tg_types.hpp"

namespace cookcookhnya::message {
using UserId = cookcookhnya::tg_types::UserId;
using MessageId = cookcookhnya::tg_types::MessageId;

std::optional<MessageId> getMessageId(UserId userId);

void addMessageId(UserId userId, MessageId messageId);

} // namespace cookcookhnya::message
