#include "view.hpp"

#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/main_menu/view.hpp"
#include "render/shopping_list/view.hpp"
#include "utils/parsing.hpp"

#include <ranges>
#include <vector>

namespace cookcookhnya::handlers::shopping_list {

using namespace render::main_menu;
using namespace render::shopping_list;

using namespace std::views;
using namespace std::ranges;

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

    if (cq.data == "search") {
        renderMainMenu(true, userId, cq.message->chat->id, bot, api);
        stateManager.put(MainMenu{});
        return;
    }

    if (cq.data == "remove") {
        using SelectableItem = ShoppingListView::SelectableItem;
        auto toDelete = state.items.getValues() | filter(&SelectableItem::selected) |
                        views::transform(&SelectableItem::ingredientId) | to<std::vector>();

        api.getShoppingListApi().remove(userId, toDelete);
        for (auto& id : toDelete)
            state.items.remove(id);

        renderShoppingList(state, userId, chatId, bot);
        return;
    }

    if (cq.data == "buy") {
        renderMainMenu(true, userId, cq.message->chat->id, bot, api);
        stateManager.put(MainMenu{});
        return;
    }

    auto mIngredientId = utils::parseSafe<api::IngredientId>(cq.data);
    if (!mIngredientId)
        return;

    if (auto* mItem = state.items[*mIngredientId]) {
        mItem->selected = !mItem->selected;
        renderShoppingList(state, userId, chatId, bot);
    }
}

} // namespace cookcookhnya::handlers::shopping_list
