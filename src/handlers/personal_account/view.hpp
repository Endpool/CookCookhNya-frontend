#pragma once

#include "handlers/common.hpp"

namespace cookcookhnya::handlers::personal_account {

void handlePersonalAccountMenuCQ(
    PersonalAccountMenu& /*unused*/, CallbackQueryRef cq, BotRef& bot, SMRef stateManager, ApiClientRef api);

} // namespace cookcookhnya::handlers::personal_account
