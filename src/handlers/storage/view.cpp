#include "view.hpp"

#include "handlers/common.hpp"
#include "render/recipes_suggestions/view.hpp"
#include "render/storage/ingredients/view.hpp"
#include "render/storage/members/view.hpp"
#include "render/storages_list/view.hpp"
#include "states.hpp"

#include <iterator>
#include <vector>

namespace cookcookhnya::handlers::storage {

using namespace render::storage::ingredients;
using namespace render::storage::members;
using namespace render::storages_list;
using namespace render::recipes_suggestions;

void handleStorageViewCQ(StorageView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;
    const size_t numOfIngredientsOnPage = 5;
    if (cq.data == "ingredients") {
        auto ingredients = api.getIngredientsApi().getStorageIngredients(userId, state.storageId);
        stateManager.put(StorageIngredientsList{
            state.storageId,
            {std::make_move_iterator(ingredients.begin()), std::make_move_iterator(ingredients.end())}});
        renderIngredientsListSearch(
            std::get<StorageIngredientsList>(*stateManager.get()), numOfIngredientsOnPage, userId, chatId, bot);
    } else if (cq.data == "members") {
        renderMemberList(true, state.storageId, userId, chatId, bot, api);
        stateManager.put(StorageMemberView{state.storageId});
    } else if (cq.data == "back") {
        renderStorageList(true, userId, chatId, bot, api);
        stateManager.put(StorageList{});
    } else if (cq.data == "wanna_eat") {
        renderRecipesSuggestion({state.storageId}, 0, userId, chatId, bot, api);
        stateManager.put(
            SuggestedRecipeList{.pageNo = 0, .storageIds = std::vector{state.storageId}, .fromStorage = true});
        return;
    }
}

} // namespace cookcookhnya::handlers::storage
