#include "list.hpp"

#include "handlers/common.hpp"
#include "render/storage_view/ingredients/search.hpp"
#include "render/storage_view/storage_view_render.hpp"
#include "tg_types.hpp"

namespace cookcookhnya::handlers::storage::ingredients {

using namespace render::storage;
using namespace render::storage::ingredients;
using namespace tg_types;

void storageIngredientsListButtonCallback(
    StorageIngredientsList& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    const auto user = cq.from->id;
    const auto chat = cq.message->chat->id;
    if (cq.data == "back") {
        renderStorageView(state.storageId, user, chat, bot, api);
        stateManager.put(StorageView{state.storageId});
    } else if (cq.data == "search") {
        MessageId message = renderStorageIngredientsSearchSend(chat, bot);
        stateManager.put(StorageIngredientsSearch{state.storageId, message, {}});
    }
}

} // namespace cookcookhnya::handlers::storage::ingredients
