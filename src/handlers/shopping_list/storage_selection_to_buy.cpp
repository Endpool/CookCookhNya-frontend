#include "storage_selection_to_buy.hpp"

#include "backend/api/shopping_lists.hpp"
#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/shopping_list/view.hpp"
#include "utils/parsing.hpp"

#include <cstddef>
#include <string_view>
#include <utility>

namespace cookcookhnya::handlers::shopping_list {

using render::shopping_list::renderShoppingList;

void handleShoppingListStorageSelectionToBuyCQ(ShoppingListStorageSelectionToBuy& state,
                                               CallbackQueryRef cq,
                                               BotRef bot,
                                               SMRef stateManager,
                                               api::ShoppingListApiRef api) {
    bot.answerCallbackQuery(cq.id);
    auto userId = cq.from->id;
    auto chatId = cq.message->chat->id;

    if (cq.data == "back") {
        renderShoppingList(state.prevState, userId, chatId, bot);
        stateManager.put(auto{std::move(state.prevState)});
        return;
    }

    const std::size_t idStrStart = sizeof("storage_") - 1;
    if (cq.data.size() <= idStrStart)
        return;
    auto mStorageId = utils::parseSafe<api::StorageId>(std::string_view{cq.data}.substr(idStrStart));
    if (!mStorageId)
        return;

    api.buy(userId, *mStorageId, state.selectedIngredients);
    for (auto& id : state.selectedIngredients)
        state.prevState.items.remove(id);
    renderShoppingList(state.prevState, userId, chatId, bot);
    stateManager.put(auto{std::move(state.prevState)});
}

} // namespace cookcookhnya::handlers::shopping_list
