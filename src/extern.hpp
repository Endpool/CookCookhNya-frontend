#pragma once

#include "tg_types.hpp"
#include <unordered_map>

namespace cookcookhnya::message {
using UserId = cookcookhnya::tg_types::UserId;
using MessageId = cookcookhnya::tg_types::MessageId;

extern std::unordered_map<UserId, MessageId> map; // NOLINT(*non-const*)

std::optional<MessageId> getMessageId(UserId userId);

void addMessageId(UserId userId, MessageId messageId);

} // namespace cookcookhnya::message
