#include "view.hpp"

#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "handlers/shopping_list/search.hpp"
#include "render/main_menu/view.hpp"
#include "render/shopping_list/search.hpp"
#include "render/shopping_list/storage_selection_to_buy.hpp"
#include "render/shopping_list/view.hpp"
#include "states.hpp"
#include "utils/parsing.hpp"

#include <ranges>
#include <utility>
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
    auto storages = api.getStoragesApi().getStoragesList(userId);

    if (cq.data == "back") {
        renderMainMenu(true, std::nullopt, userId, cq.message->chat->id, bot, api);
        stateManager.put(MainMenu{});
        return;
    }

    if (cq.data == "search") {
        ShoppingListIngredientSearch newState{.prevState = std::move(state), .query = "", .pagination = {}, .page = {}};
        renderShoppingListIngredientSearch(newState, searchPageSize, userId, chatId, bot);
        stateManager.put(std::move(newState));
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
        using SelectableItem = ShoppingListView::SelectableItem;
        auto toBuy = state.items.getValues() | filter(&SelectableItem::selected) |
                     views::transform(&SelectableItem::ingredientId) | to<std::vector>();
        if (storages.size() == 1) {
            api.getShoppingListApi().buy(userId, storages[0].id, toBuy);
            for (auto& id : toBuy)
                state.items.remove(id);
            renderShoppingList(state, userId, chatId, bot);
        } else {
            auto newState = ShoppingListStorageSelectionToBuy{
                .prevState = std::move(state), .selectedIngredients = std::move(toBuy), .storages = storages};
            renderShoppingListStorageSelectionToBuy(newState, userId, chatId, bot);
            stateManager.put(std::move(newState));
        }
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
