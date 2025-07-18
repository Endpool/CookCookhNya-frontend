#include "view.hpp"

#include "handlers/common.hpp"
#include "render/recipes_suggestions/view.hpp"
#include "render/storage/ingredients/view.hpp"
#include "render/storage/members/view.hpp"
#include "render/storages_list/view.hpp"
#include "states.hpp"

#include <cstddef>
#include <ranges>
#include <utility>

namespace cookcookhnya::handlers::storage {

using namespace render::storage::ingredients;
using namespace render::storage::members;
using namespace render::storages_list;
using namespace render::recipes_suggestions;
using namespace std::views;

const std::size_t numOfIngredientsOnPage = 5;

void handleStorageViewCQ(StorageView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    if (cq.data == "ingredients") {
        auto ingredients = api.getIngredientsApi().getStorageIngredients(userId, state.storageId);
        auto newState = StorageIngredientsList{state.storageId, ingredients | as_rvalue, ""};
        renderIngredientsListSearch(newState, numOfIngredientsOnPage, userId, chatId, bot);
        stateManager.put(std::move(newState));
    } else if (cq.data == "members") {
        renderMemberList(true, state.storageId, userId, chatId, bot, api);
        stateManager.put(StorageMemberView{state.storageId});
    } else if (cq.data == "back") {
        renderStorageList(true, userId, chatId, bot, api);
        stateManager.put(StorageList{});
    } else if (cq.data == "wanna_eat") {
        renderRecipesSuggestion({state.storageId}, 0, userId, chatId, bot, api);
        stateManager.put(SuggestedRecipeList{.pageNo = 0, .storageIds = {state.storageId}, .fromStorage = true});
    }
}

} // namespace cookcookhnya::handlers::storage
