#include "start.hpp"

#include "backend/id_types.hpp"
#include "backend/models/user.hpp"
#include "handlers/common.hpp"
#include "render/main_menu/view.hpp"
#include "states.hpp"

#include <optional>
#include <string>
#include <utility>

namespace cookcookhnya::handlers::initial {

using namespace render::main_menu;
using namespace std::literals;

void start(MessageRef m, BotRef bot, SMRef stateManager, ApiClientRef api) {
    auto userId = m.from->id;
    renderMainMenu(false, m.from->id, m.chat->id, bot, api);
    stateManager.put(MainMenu{});
    std::string fullname = m.from->firstName;
    if (!m.from->lastName.empty()) {
        fullname += ' ';
        fullname += m.from->lastName;
    }
    std::optional<std::string> alias;
    if (!m.from->username.empty())
        alias = m.from->username;

    api.getUsersApi().updateInfo(
        userId,
        api::models::user::UpdateUserInfoBody{.alias = std::move(m.from->username), .fullname = std::move(fullname)});

    auto startText = m.text;
    const int hashPos = "/start "sv.size();
    if (startText.size() > hashPos - 1) {
        auto hash = std::string(m.text).substr(hashPos);
        api.getStoragesApi().activate(userId, hash);
    }
};

void handleNoState(MessageRef m, BotRef bot) {
    if (m.text.starts_with("/start"))
        return;
    bot.sendMessage(m.chat->id, "Use /start please");
};

} // namespace cookcookhnya::handlers::initial
