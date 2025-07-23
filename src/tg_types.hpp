#pragma once

#include <tg_stater/tg_types.hpp>
#include <tgbot/types/Message.h>

namespace cookcookhnya::tg_types {
// privet
using UserId = tg_stater::UserIdT;
using ChatId = tg_stater::ChatIdT;
using MessageId = decltype(TgBot::Message::messageId);

} // namespace cookcookhnya::tg_types
