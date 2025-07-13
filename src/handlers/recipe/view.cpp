#include "view.hpp"

#include "handlers/common.hpp"
#include "render/recipe/add_storage.hpp"
#include "render/recipes_suggestions/view.hpp"
#include "render/shopping_list/create.hpp"

#include <string>

namespace cookcookhnya::handlers::recipe {

using namespace render::recipes_suggestions;
using namespace render::shopping_list;
using namespace render::recipe;

void handleRecipeView(RecipeView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    std::string data = cq.data;
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    if (data == "start_cooking") {
        // TODO: add state of begginig of cooking
        return;
    }
    if (data == "make_product_list") {
        auto ingredientsInList = renderShoppingListCreation(state.storageIds, state.recipeId, userId, chatId, bot, api);
        stateManager.put(ShoppingListCreation{.storageIdsFrom = state.storageIds,
                                              .recipeIdFrom = state.recipeId,
                                              .ingredientIdsInList = ingredientsInList,
                                              .fromStorage = state.fromStorage,
                                              .pageNo = state.pageNo});
        bot.answerCallbackQuery(cq.id);
        return;
    }
    if (data == "back_from_recipe_view") {
        editRecipesSuggestion(state.storageIds, state.pageNo, userId, chatId, bot, api);
        stateManager.put(SuggestedRecipeList{
            .pageNo = state.pageNo, .storageIds = state.storageIds, .fromStorage = state.fromStorage});
        bot.answerCallbackQuery(cq.id);
        return;
    }

    if (data[0] == '?') {
        renderStorageSuggestion(state.storageIds, state.recipeId, userId, chatId, bot, api);
        stateManager.put(RecipeStorageAddition{.storageIds = state.storageIds,
                                               .recipeId = state.recipeId,
                                               .fromStorage = state.fromStorage,
                                               .pageNo = state.pageNo});
        return;
    }
}

} // namespace cookcookhnya::handlers::recipe
