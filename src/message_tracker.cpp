#include "message_tracker.hpp"

#include <optional>
#include <unordered_map>

namespace cookcookhnya::message {

using namespace cookcookhnya::tg_types;

namespace {

std::unordered_map<UserId, MessageId> map{}; // NOLINT(*non-const*)

} // namespace

std::optional<MessageId> getMessageId(UserId userId) {
    auto it = map.find(userId);
    return it != map.end() ? std::optional{it->second} : std::nullopt;
}

void addMessageId(UserId userId, MessageId messageId) {
    map.insert_or_assign(userId, messageId);
}

} // namespace cookcookhnya::message
