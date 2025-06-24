#include "storage_view.hpp"

#include "handlers/common.hpp"
#include "render/storage_list/storage_list_render.hpp"
#include "render/storage_view/ingredients/list.hpp"
#include "render/storage_view/storage_members_render.hpp"
#include "states.hpp"

#include <vector>

namespace cookcookhnya::handlers::storage_view {

using namespace render::storage::ingredients;
using namespace render::storage::member_list;
using namespace render::storage_list;

void storageViewButtonCallback(
    StorageView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;
    if (cq.data == "storage_view_explore") {
        stateManager.put(StorageIngredientsList{state.storageId});
        renderIngredientsList(state.storageId, userId, chatId, bot, api);
    } else if (cq.data == "storage_view_members") {
        renderMemberList(state.storageId, userId, chatId, bot, api);
        stateManager.put(StorageMemberView{state.storageId});
    } else if (cq.data == "storage_view_back") {
        renderStorageList(userId, chatId, bot, api);
        stateManager.put(StorageList{});
    } else if (cq.data == "storage_view_what_to_cook") {
        // renderRecipesSuggetion();
        stateManager.put(SuggestedRecipeList{std::vector{state.storageId}});
        return;
    }
}

} // namespace cookcookhnya::handlers::storage_view
