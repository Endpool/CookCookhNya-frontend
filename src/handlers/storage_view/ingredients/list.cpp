#include "list.hpp"

#include "handlers/common.hpp"
#include "render/storage_view/ingredients/edit.hpp"
#include "render/storage_view/storage_view_render.hpp"
#include "states.hpp"

namespace cookcookhnya::handlers::storage::ingredients {

void storageIngredientsListButtonCallback(
    StorageIngredientsView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    auto user = cq.from->id;
    auto chat = cq.from->id;
    if (cq.data == "back") {
        render::storage::renderStorageView(state.storageId, user, chat, bot, api);
        stateManager.put(StorageView{state.storageId});
    } else if (cq.data == "edit") {
        render::storage::ingredients::renderIngredientsEdit(chat, bot, api);
        stateManager.put(StorageIngredientsEdit{state.storageId});
    }
}

} // namespace cookcookhnya::handlers::storage::ingredients
