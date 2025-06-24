#pragma once

#include "backend/api/api.hpp"
#include "backend/api/storages.hpp"
#include "backend/api/users.hpp"
#include "states.hpp"

#include <tg_stater/handler/type.hpp>
#include <tgbot/Api.h>
#include <tgbot/types/CallbackQuery.h>
#include <tgbot/types/Message.h>

namespace cookcookhnya::forHandlers { //*forhandlers

using namespace states;

using ApiClientRef = const api::ApiClient&;
using StorageApiRef = const api::StoragesApi&;
using UserApiRef = const api::UsersApi&;

using BotRef = const TgBot::Api&;
using SMRef = const states::StateManager&;
using MessageRef = const TgBot::Message&;
using CallbackQueryRef = const TgBot::CallbackQuery&;

using NoState = tg_stater::HandlerTypes::NoState;
using AnyState = tg_stater::HandlerTypes::AnyState;

} // namespace cookcookhnya::forHandlers
