#include "view.hpp"

#include "handlers/common.hpp"
#include "render/recipes_suggestion/recipes_suggestion_render.hpp"
#include "render/storage_list/storage_list_render.hpp"
#include "render/storage_view/ingredients/list.hpp"
#include "render/storage_view/storage_members_render.hpp"
#include "states.hpp"

#include <vector>

namespace cookcookhnya::handlers::storage_view {

using namespace render::storage::ingredients;
using namespace render::storage::member_list;
using namespace render::storage_list;
using namespace render::recipes_suggestion;

void storageViewButtonCallback(
    StorageView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;
    if (cq.data == "storage_view_explore") {
        stateManager.put(StorageIngredientsList{state.storageId});
        renderIngredientsList(state.storageId, userId, chatId, bot, api);
    } else if (cq.data == "storage_view_members") {
        renderMemberList(true, state.storageId, userId, chatId, bot, api);
        stateManager.put(StorageMemberView{state.storageId});
    } else if (cq.data == "storage_view_back") {
        renderStorageList(true, userId, chatId, bot, api);
        stateManager.put(StorageList{});
    } else if (cq.data == "storage_view_wanna_eat") {
        editRecipesSuggestion({state.storageId}, 0, userId, chatId, bot, api);
        stateManager.put(
            SuggestedRecipeList{.pageNo = 0, .storageIds = std::vector{state.storageId}, .fromStorage = true});
        return;
    }
}

} // namespace cookcookhnya::handlers::storage_view
