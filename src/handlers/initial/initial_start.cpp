#include "initial_start.hpp"

#include "handlers/type_refs.hpp"
#include "render/storage_list/storage_list_render.hpp"

#include <optional>
#include <string>
#include <utility>

namespace cookcookhnya::handlers::init {
using namespace cookcookhnya::handlers;
using namespace cookcookhnya::render::storageList;

void start(MessageRef m, BotRef bot, SMRef stateManager, ApiClientRef api) {
    stateManager.put(StorageList{});
    renderStorageList(m.from->id, m.chat->id, bot, api);

    std::string fullname = m.from->firstName;
    if (!m.from->lastName.empty()) {
        fullname += ' ';
        fullname += m.from->lastName;
    }

    std::optional<std::string> alias;
    if (!m.from->username.empty())
        alias = m.from->username;

    api.getUsers().updateInfo(
        m.from->id,
        models::user::UpdateUserInfoBody{.alias = std::move(m.from->username), .fullname = std::move(fullname)});
};

void handleNoState(MessageRef m, BotRef bot) {
    if (m.text.starts_with("/start"))
        return;
    bot.sendMessage(m.chat->id, "Use /start please");
};
} // namespace cookcookhnya::handlers::init
