#include "shopping_list.hpp"

#include "backend/api/api.hpp"
#include "handlers/common.hpp"
#include "message_tracker.hpp"
#include "render/shopping_list/view.hpp"
#include "states.hpp"

#include <utility>

namespace cookcookhnya::handlers::commands {

using namespace render::shopping_list;

void handleShoppingListCmd(MessageRef m, BotRef bot, SMRef stateManager, api::ApiClientRef api) {
    const auto userId = m.from->id;

    const bool hasStorages = !api.getStoragesApi().getStoragesList(userId).empty();
    auto items = api.getShoppingListApi().get(userId);

    auto newState = ShoppingListView{.items = std::move(items), .canBuy = hasStorages};
    message::deleteMessageId(userId);
    renderShoppingList(newState, userId, m.chat->id, bot);
    stateManager.put(newState);
};

} // namespace cookcookhnya::handlers::commands
