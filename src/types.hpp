#include <cstdio>
#include <httplib.h>
#include <tg_stater/bot.hpp>
#include <tg_stater/dependencies.hpp>
#include <tg_stater/handler/event.hpp>
#include <tg_stater/handler/handler.hpp>
#include <tg_stater/handler/type.hpp>
#include <tgbot/Api.h>
#include <tgbot/Bot.h>
#include <tgbot/types/Message.h>


#include "states.hpp"

using UserId = tg_stater::UserIdT;
using namespace TgBot;
using namespace tg_stater;
using namespace states;

using ChatId = tg_stater::ChatIdT;
using SMRef = const StateManager& ;
using CallbackQueryRef = const CallbackQuery&;
using MessageRef = const Message&;
using BotRef = const Api&;

