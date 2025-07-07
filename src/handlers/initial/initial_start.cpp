#include "initial_start.hpp"

#include "backend/models/user.hpp"
#include "handlers/common.hpp"
#include "render/main_menu/main_menu_render.hpp"
#include "states.hpp"

#include <optional>
#include <string>
#include <utility>

namespace cookcookhnya::handlers::init {

using namespace render::main_menu;

void start(MessageRef m, BotRef bot, SMRef stateManager, ApiClientRef api) {
    renderMainMenu(false, m.from->id, m.chat->id, bot, api);
    stateManager.put(MainMenu{});
    std::string fullname = m.from->firstName;
    if (!m.from->lastName.empty()) {
        fullname += ' ';
        fullname += m.from->lastName;
    }
    auto startText = m.text;
    const int hashPos = 7;
    if (startText.size() > hashPos - 1) {
        auto hash = std::string(m.text).substr(hashPos); // NOLINT
        // TODO: if user used start with hash then send request to add to storage
    }
    std::optional<std::string> alias;
    if (!m.from->username.empty())
        alias = m.from->username;

    api.getUsersApi().updateInfo(
        m.from->id,
        api::models::user::UpdateUserInfoBody{.alias = std::move(m.from->username), .fullname = std::move(fullname)});
};

void handleNoState(MessageRef m, BotRef bot) {
    if (m.text.starts_with("/start"))
        return;
    bot.sendMessage(m.chat->id, "Use /start please");
};

} // namespace cookcookhnya::handlers::init
