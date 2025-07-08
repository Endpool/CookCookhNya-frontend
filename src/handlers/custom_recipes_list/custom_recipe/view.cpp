#include "view.hpp"

#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/recipes_suggestion/recipe/view.hpp"
#include "render/recipes_suggestion/suggest.hpp"
#include "render/shopping_list/create.hpp"
#include "utils.hpp"

#include <string>

namespace cookcookhnya::handlers::recipe_view {

using namespace render::recipe_view;
using namespace render::recipes_suggestion;
using namespace render::shopping_list_create;

void handleRecipeView(RecipeView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    std::string data = cq.data;
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    if (data == "startCooking") {
        // TODO: add state of begginig of cooking
        return;
    }

    if (data == "backFromRecipeView") {
        editRecipesSuggestion(state.storageIds, 0, userId, chatId, bot, api);
        stateManager.put(
            SuggestedRecipeList{.pageNo = 0, .storageIds = state.storageIds, .fromStorage = state.fromStorage});
        bot.answerCallbackQuery(cq.id);
        return;
    }

    if (data[0] == '+') {
        auto newStorageIdStr =
            data.substr(1, data.size()); // Here we got all selected storages and new one as last in string
        auto newStorageId = utils::parseSafe<api::StorageId>(newStorageIdStr);
        if (newStorageId) {
            state.storageIds.push_back(*newStorageId);
            renderStorageSuggestion(state.storageIds, state.recipeId, userId, chatId, bot, api);
        }
    }
}

} // namespace cookcookhnya::handlers::recipe_view
