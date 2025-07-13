#include "handlers/personal_account/view.hpp"

#include "handlers/common.hpp"
#include "render/custom_recipes_list/view.hpp"
#include "render/main_menu/view.hpp"
#include "render/personal_account/ingredients/view.hpp"

#include <string>

namespace cookcookhnya::handlers::personal_account_view {

using namespace render::custom_recipes_list;
using namespace render::main_menu;
using namespace render::view_custom_ingredients;

void personalAccount(PersonalAccountMenu& /**/, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    const std::string data = cq.data;
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    if (data == "recipes") {
        renderCustomRecipesList(0, userId, chatId, bot, api);
        stateManager.put(CustomRecipesList{.pageNo = 0});
    }
    if (data == "back") {
        renderMainMenu(true, userId, chatId, bot, api);
        stateManager.put(MainMenu{});
        return;
    }
    if (data == "ingredients") {
        renderCustomIngredientsList(true, userId, chatId, bot, api);
        stateManager.put(CustomIngredientsList{});
        return;
    }
}

} // namespace cookcookhnya::handlers::personal_account_view
