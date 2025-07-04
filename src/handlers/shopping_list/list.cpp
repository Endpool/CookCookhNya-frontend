#include "list.hpp"

#include "handlers/common.hpp"
#include "render/storage_list/storage_list_render.hpp"

namespace cookcookhnya::handlers::shopping_list {

using namespace render::storage_list;

void shoppingListButtonCallback(
    ShoppingListView& /*unused*/, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    if (cq.data == "back") {
        renderStorageList(true, cq.from->id, cq.message->chat->id, bot, api);
        stateManager.put(StorageList{});
        return;
    }
}

} // namespace cookcookhnya::handlers::shopping_list
