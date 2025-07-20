#include "shopping_list.hpp"

#include "handlers/common.hpp"
#include "message_tracker.hpp"
#include "render/shopping_list/view.hpp"
#include "states.hpp"

namespace cookcookhnya::handlers::commands {

using namespace render::shopping_list;

void handleShoppingListCmd(MessageRef m, BotRef bot, SMRef stateManager) {
    auto newState = ShoppingListView{};
    message::deleteMessageId(m.from->id);
    renderShoppingList(newState, m.from->id, m.chat->id, bot);
    stateManager.put(newState);
};

} // namespace cookcookhnya::handlers::commands
