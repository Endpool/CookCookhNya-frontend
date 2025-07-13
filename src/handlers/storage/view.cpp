#include "view.hpp"

#include "handlers/common.hpp"
#include "render/recipes_suggestion/suggest.hpp"
#include "render/storage_list/view.hpp"
#include "render/storage_view/ingredients/view.hpp"
#include "render/storage_view/members/view.hpp"
#include "states.hpp"

#include <iterator>
#include <vector>

namespace cookcookhnya::handlers::storage {

using namespace render::storage::ingredients;
using namespace render::storage::member_list;
using namespace render::storage_list;
using namespace render::recipes_suggestion;

void storageViewButtonCallback(
    StorageView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;
    if (cq.data == "ingredients") {
        auto ingredients = api.getIngredientsApi().getStorageIngredients(userId, state.storageId);
        stateManager.put(StorageIngredientsList{
            state.storageId,
            {std::make_move_iterator(ingredients.begin()), std::make_move_iterator(ingredients.end())}});
        renderIngredientsListSearch(std::get<StorageIngredientsList>(*stateManager.get()), userId, chatId, bot);
    } else if (cq.data == "members") {
        renderMemberList(true, state.storageId, userId, chatId, bot, api);
        stateManager.put(StorageMemberView{state.storageId});
    } else if (cq.data == "back") {
        renderStorageList(true, userId, chatId, bot, api);
        stateManager.put(StorageList{});
    } else if (cq.data == "wanna_eat") {
        editRecipesSuggestion({state.storageId}, 0, userId, chatId, bot, api);
        stateManager.put(
            SuggestedRecipeList{.pageNo = 0, .storageIds = std::vector{state.storageId}, .fromStorage = true});
        return;
    }
}

} // namespace cookcookhnya::handlers::storage
