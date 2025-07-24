#include "handlers/personal_account/view.hpp"

#include "backend/api/api.hpp"
#include "handlers/common.hpp"
#include "render/main_menu/view.hpp"
#include "render/personal_account/ingredients_list/view.hpp"
#include "render/personal_account/publication_history.hpp"
#include "render/personal_account/recipes_list/view.hpp"

#include <cstddef>
#include <optional>
#include <string>

namespace cookcookhnya::handlers::personal_account {

using namespace render::main_menu;
using namespace render::personal_account::recipes_list;
using namespace render::personal_account::ingredients;
using namespace render::personal_account;

const std::size_t numOfHistoryInstances = 10;

void handlePersonalAccountMenuCQ(
    PersonalAccountMenu& /**/, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::ApiClientRef api) {
    const std::string data = cq.data;
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    if (data == "recipes") {
        renderCustomRecipesList(0, userId, chatId, bot, api);
        stateManager.put(CustomRecipesList{.pageNo = 0});
        return;
    }
    if (data == "back") {
        renderMainMenu(true, std::nullopt, userId, chatId, bot, api);
        stateManager.put(MainMenu{});
        return;
    }
    if (data == "ingredients") {
        renderCustomIngredientsList(true, userId, chatId, bot, api);
        stateManager.put(CustomIngredientsList{});
        return;
    }
    if (data == "history") {
        renderRequestHistory(userId, 0, numOfHistoryInstances, chatId, bot, api);
        stateManager.put(TotalPublicationHistory{.pageNo = 0});
        return;
    }
}

} // namespace cookcookhnya::handlers::personal_account
