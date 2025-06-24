#include "list.hpp"

#include "handlers/common.hpp"
#include "render/storage_view/ingredients/search.hpp"
#include "render/storage_view/storage_view_render.hpp"
#include "tg_types.hpp"

namespace cookcookhnya::handlers::storage::ingredients {

void storageIngredientsListButtonCallback(
    StorageIngredientsList& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    auto user = cq.from->id;
    auto chat = cq.from->id;
    if (cq.data == "back") {
        render::storage::renderStorageView(state.storageId, user, chat, bot, api);
        stateManager.put(StorageView{state.storageId});
    } else if (cq.data == "search") {
        tg_types::MessageId message = render::storage::ingredients::renderStorageIngredientsSearchSend(chat, bot, api);
        stateManager.put(StorageIngredientsSearch{state.storageId, message});
    }
}

} // namespace cookcookhnya::handlers::storage::ingredients
