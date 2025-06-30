#include "extern.hpp"
#include <optional>
#include <unordered_map>

namespace cookcookhnya::message {

using namespace cookcookhnya::tg_types;

std::unordered_map<UserId, MessageId> map = {}; // NOLINT(*non-const*)

std::optional<MessageId> getMessageId(UserId userId) {
    return map.find(userId)->second;
}
void addMessageId(UserId userId, MessageId messageId) {
    map.insert_or_assign(userId, messageId);
}

} // namespace cookcookhnya::message
