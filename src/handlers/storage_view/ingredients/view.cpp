#include "view.hpp"

#include "handlers/common.hpp"
#include "render/storage_view/ingredients/search.hpp"
#include "render/storage_view/view.hpp"

namespace cookcookhnya::handlers::storage::ingredients {

using namespace render::storage;
using namespace render::storage::ingredients;
using namespace tg_types;

void storageIngredientsListButtonCallback(
    StorageIngredientsList& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    const auto userId = cq.from->id;
    const auto chatId = cq.message->chat->id;
    if (cq.data == "back") {
        renderStorageView(state.storageId, userId, chatId, bot, api);
        stateManager.put(StorageView{state.storageId});
    } else if (cq.data == "search") {
        renderStorageIngredientsSearch(chatId, userId, bot);
        stateManager.put(StorageIngredientsSearch{state.storageId, {}, 0, 0});
    }
}

} // namespace cookcookhnya::handlers::storage::ingredients
