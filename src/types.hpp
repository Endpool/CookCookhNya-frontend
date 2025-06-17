#pragma once

#include <tg_stater/tg_types.hpp>
#include <tgbot/Api.h>
#include <tgbot/types/InlineKeyboardButton.h>
#include <tgbot/types/Message.h>

#include <vector>

using UserId = int;
using StorageId = int;
using ChatId = tg_stater::ChatIdT;
using MessageId = decltype(TgBot::Message::messageId);

using BotRef = const TgBot::Api&;
using InlineKeyboard = std::vector<std::vector<TgBot::InlineKeyboardButton::Ptr>>;