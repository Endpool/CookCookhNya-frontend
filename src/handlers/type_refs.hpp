#pragma once

#include "backend/api/api.hpp"
#include "backend/api/storages.hpp"
#include "backend/api/users.hpp"
#include "states.hpp"

#include <tg_stater/handler/event.hpp>
#include <tg_stater/handler/handler.hpp>
#include <tg_stater/handler/type.hpp>
#include <tg_stater/tg_types.hpp>
#include <tgbot/Api.h>
#include <tgbot/types/CallbackQuery.h>
#include <tgbot/types/InlineKeyboardButton.h>
#include <tgbot/types/Message.h>

namespace cookcookhnya::forHandlers { //*forhandlers

using namespace TgBot;
using namespace tg_stater;
using namespace states;
using namespace cookcookhnya::api;

using ApiClientRef = const ApiClient&;
using StorageApiRef = const StoragesApi&;
using UserApiRef = const UsersApi&;
using MessageRef = const Message&;
using CallbackQueryRef = const CallbackQuery&;
using SMRef = const StateManager&;
using NoState = HandlerTypes::NoState;
using AnyState = HandlerTypes::AnyState;
using BotRef = const Api&;

} // namespace cookcookhnya::forHandlers
