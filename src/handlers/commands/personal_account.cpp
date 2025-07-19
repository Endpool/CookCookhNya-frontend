#include "personal_account.hpp"

#include "handlers/common.hpp"
#include "message_tracker.hpp"
#include "render/personal_account/view.hpp"
#include "states.hpp"

namespace cookcookhnya::handlers::commands {

using namespace render::personal_account;

void handlePersonalAccountCmd(MessageRef m, BotRef bot, SMRef stateManager) {
    message::deleteMessageId(m.from->id);
    renderPersonalAccountMenu(m.from->id, m.chat->id, bot);
    stateManager.put(PersonalAccountMenu{});
};

} // namespace cookcookhnya::handlers::commands
