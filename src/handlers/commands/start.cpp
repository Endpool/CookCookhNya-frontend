#include "start.hpp"

#include "backend/api/api.hpp"
#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "message_tracker.hpp"
#include "render/main_menu/view.hpp"
#include "render/recipe/view.hpp"
#include "utils/parsing.hpp"
#include "utils/uuid.hpp"

#include <optional>
#include <string>
#include <string_view>
#include <utility>

namespace cookcookhnya::handlers::commands {

using namespace render::main_menu;
using namespace render::recipe;
using namespace api::models::user;
using namespace std::literals;

void handleStartCmd(MessageRef m, BotRef bot, SMRef stateManager, api::ApiClientRef api) {
    const auto userId = m.from->id;
    const auto chatId = m.chat->id;

    std::string fullName = m.from->firstName;
    if (!m.from->lastName.empty()) {
        fullName += ' ';
        fullName += m.from->lastName;
    }

    std::optional<std::string> alias;
    if (!m.from->username.empty())
        alias = m.from->username;

    api.getUsersApi().updateInfo(userId, {.alias = std::move(alias), .fullName = std::move(fullName)});

    if (!m.text.starts_with("/start ")) {
        // default case
        renderMainMenu(false, std::nullopt, userId, chatId, bot, api);
        stateManager.put(MainMenu{});
        return;
    }
    const std::string_view payload = std::string_view{m.text}.substr("/start "sv.size());

    if (payload.starts_with("invite_")) {
        const std::string_view hash = payload.substr("invite_"sv.size());
        auto storage = api.getStoragesApi().activate(userId, api::InvitationId{hash});
        if (!storage)
            return;
        renderMainMenu(false, storage->name, userId, chatId, bot, api);
        stateManager.put(MainMenu{});
        return;
    }

    if (payload.starts_with("recipe_")) {
        const auto mRecipeId = utils::parseSafe<Uuid>(payload.substr("recipe_"sv.size()));
        if (!mRecipeId)
            return;
        auto recipe = api.getRecipesApi().get(userId, *mRecipeId);
        message::deleteMessageId(userId);
        renderRecipeView(recipe, *mRecipeId, userId, chatId, bot);
        stateManager.put(RecipeView{.prevState = std::nullopt, .recipe = std::move(recipe)});
        return;
    }
};

void handleNoState(MessageRef m, BotRef bot) {
    if (m.text.starts_with("/start"))
        return;
    bot.sendMessage(m.chat->id, "Use /start please");
};

} // namespace cookcookhnya::handlers::commands
