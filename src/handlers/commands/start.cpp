#include "start.hpp"

#include "backend/id_types.hpp"
#include "backend/models/user.hpp"
#include "handlers/common.hpp"
#include "render/main_menu/view.hpp"
#include "states.hpp"

#include <optional>
#include <string>
#include <string_view>
#include <utility>

namespace cookcookhnya::handlers::commands {

using namespace render::main_menu;
using namespace std::literals;

void handleStartCmd(MessageRef m, BotRef bot, SMRef stateManager, ApiClientRef api) {
    auto userId = m.from->id;
    std::string fullName = m.from->firstName;
    if (!m.from->lastName.empty()) {
        fullName += ' ';
        fullName += m.from->lastName;
    }
    std::optional<std::string> alias;
    if (!m.from->username.empty())
        alias = m.from->username;

    api.getUsersApi().updateInfo(
        userId,
        api::models::user::UpdateUserInfoBody{.alias = std::move(m.from->username), .fullName = std::move(fullName)});

    auto startText = m.text;
    const int hashPos = "/start "sv.size();
    if (startText.size() > hashPos - 1) {
        auto hash = std::string(m.text).substr(hashPos);
        auto storage = api.getStoragesApi().activate(userId, hash);
        renderMainMenu(false, storage->name, m.from->id, m.chat->id, bot, api);
        stateManager.put(MainMenu{});
        return;
    }
    renderMainMenu(false, std::nullopt, m.from->id, m.chat->id, bot, api);
    stateManager.put(MainMenu{});
};

void handleNoState(MessageRef m, BotRef bot) {
    if (m.text.starts_with("/start"))
        return;
    bot.sendMessage(m.chat->id, "Use /start please");
};

} // namespace cookcookhnya::handlers::commands
