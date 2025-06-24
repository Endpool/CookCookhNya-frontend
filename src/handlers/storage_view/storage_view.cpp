#include "storage_view.hpp"

#include "render/ingredients_view/ingredients_render.hpp"
#include "render/storage_list/storage_list_render.hpp"
#include "render/storage_view/storage_members_render.hpp"

namespace cookcookhnya::handlers::storage_view {

using namespace render::view_ingredients;
using namespace render::view_storage_members;
using namespace render::storage_list;

void storageViewButtonCallback(
    StorageView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi) {
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;
    if (cq.data == "explore") {
        // stateManager.put(IngredientsView{state.storageId}); temporarily not available
        // renderIngredientsList(state.storageId, userId, chatId, bot);
    } else if (cq.data == "members") {
        renderMemberList(state.storageId, userId, chatId, bot, storageApi);
        stateManager.put(StorageMemberView{state.storageId});
    } else if (cq.data == "back") {
        renderStorageList(userId, chatId, bot, storageApi);
        stateManager.put(StorageList{});
    }
}

} // namespace cookcookhnya::handlers::storage_view
