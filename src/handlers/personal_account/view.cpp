#include "view.hpp"

#include "handlers/common.hpp"
#include "render/main_menu/view.hpp"
#include "render/personal_account/ingredients/view.hpp"

namespace cookcookhnya::handlers::personal_account_view {

using namespace render::view_custom_ingredients;
using namespace render::main_menu;

void personalAccountButtonHandler(
    PersonalAccountMenu& /*unused*/, CallbackQueryRef cq, BotRef& bot, SMRef stateManager, ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    auto userId = cq.from->id;
    auto chatId = cq.message->chat->id;
    if (cq.data == "my_ingredients") {
        renderCustomIngredientsList(true, userId, chatId, bot, api);
        stateManager.put(CustomIngredientsList{});
        return;
    }
    if (cq.data == "my_recipes") {
        // renderCustomRecipesList(true, userId, chatId, bot, api);
        // stateManager.put(MyRecipes{});
        return;
    }
    if (cq.data == "back") {
        renderMainMenu(true, cq.from->id, cq.message->chat->id, bot, api);
        stateManager.put(MainMenu{});
        return;
    }
}
} // namespace cookcookhnya::handlers::personal_account_view
