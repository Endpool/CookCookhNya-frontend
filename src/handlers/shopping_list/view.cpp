#include "view.hpp"

#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/main_menu/view.hpp"
#include "render/shopping_list/view.hpp"
#include "utils/parsing.hpp"

namespace cookcookhnya::handlers::shopping_list {

using namespace render::main_menu;
using namespace render::shopping_list;

void handleShoppingListViewCQ(
    ShoppingListView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    auto userId = cq.from->id;
    auto chatId = cq.message->chat->id;
    if (cq.data == "back") {
        renderMainMenu(true, userId, cq.message->chat->id, bot, api);
        stateManager.put(MainMenu{});
        return;
    }

    auto mIngredientId = utils::parseSafe<api::IngredientId>(cq.data);
    if (!mIngredientId)
        return;

    api.getShoppingListApi().remove(userId, {*mIngredientId});
    state.items.remove(*mIngredientId);
    renderShoppingList(state.items.getAll(), userId, chatId, bot);
}

} // namespace cookcookhnya::handlers::shopping_list
