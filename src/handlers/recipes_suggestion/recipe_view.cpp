#include "recipe_view.hpp"

#include "handlers/common.hpp"
#include "render/recipes_suggestion/recipes_suggestion_render.hpp"

#include <string>
#include <utility>

namespace cookcookhnya::handlers::recipe_view {

using render::recipes_suggestion::renderRecipesSuggestion;

void handleRecipeView(RecipeView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    const std::string& data = cq.data;

    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    if (data == "startCooking") {
        // TODO: add state of begginig of cooking
        return;
    }
    if (data == "makeReceipt") {
        // TODO: add state of rec
    }
    if (data == "backFromRecipeView") {
        renderRecipesSuggestion(state.storageIds, 1, userId, chatId, bot, api);
        stateManager.put(
            SuggestedRecipeList{.pageNo = 1, .storageIds = std::move(state.storageIds), .fromStorage = false});
        return;
    }
}

} // namespace cookcookhnya::handlers::recipe_view
