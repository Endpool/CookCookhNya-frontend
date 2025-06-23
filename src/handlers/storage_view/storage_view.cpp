#include "storage_view.hpp"
#include "render/ingredients_view/ingredients_render.hpp"
#include "render/storage_list/storage_list_render.hpp"
#include "render/storage_view/storage_members_render.hpp"

namespace cookcookhnya::handlers::storageView {
using namespace cookcookhnya::forHandlers;
using namespace cookcookhnya::render::viewIngredients;
using namespace cookcookhnya::render::viewStorageMembers;
using namespace cookcookhnya::render::storageList;

void storageViewButtonCallback(
    StorageView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi) {
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;
    if (cq.data == "explore") {
        // stateManager.put(IngredientsView{state.storageId}); temporarily not available
        renderIngredientsList(state.storageId, userId, chatId, bot);
    } else if (cq.data == "members") {
        stateManager.put(StorageMemberView{state.storageId});
        renderMemberList(state.storageId, userId, chatId, bot, storageApi);
    } else if (cq.data == "back") {
        stateManager.put(StorageList{});
        renderStorageList(userId, chatId, bot, storageApi);
    }
}
} // namespace cookcookhnya::handlers::storageView
