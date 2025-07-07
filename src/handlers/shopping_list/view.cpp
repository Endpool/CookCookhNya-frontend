#include "view.hpp"

#include "handlers/common.hpp"
#include "render/main_menu/view.hpp"

namespace cookcookhnya::handlers::shopping_list {

using namespace render::main_menu;

void shoppingListButtonCallback(
    ShoppingListView& /*unused*/, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    if (cq.data == "back") {
        renderMainMenu(true, cq.from->id, cq.message->chat->id, bot, api);
        stateManager.put(MainMenu{});
        return;
    }
}

} // namespace cookcookhnya::handlers::shopping_list
