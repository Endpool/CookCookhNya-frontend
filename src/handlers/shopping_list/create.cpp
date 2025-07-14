#include "create.hpp"

#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/recipe/view.hpp"
#include "render/shopping_list/create.hpp"
#include "utils/parsing.hpp"

#include <string>

namespace cookcookhnya::handlers::shopping_list {

using namespace render::shopping_list;
using namespace render::recipe;

void handleShoppingListCreationCQ(
    ShoppingListCreation& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    std::string data = cq.data;
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    if (data == "back") {
        renderRecipeView(state.storageIdsFrom, state.recipeIdFrom, userId, chatId, bot, api);
        stateManager.put(RecipeView{.storageIds = state.storageIdsFrom,
                                    .recipeId = state.recipeIdFrom,
                                    .fromStorage = state.fromStorage,
                                    .pageNo = state.pageNo});
        bot.answerCallbackQuery(cq.id);
        return;
    }
    if (data == "confirm") {
        // Put ingredients in list
        auto shoppingApi = api.getShoppingListApi();
        shoppingApi.put(userId, state.ingredientIdsInList);

        // Return to previous state
        renderRecipeView(state.storageIdsFrom, state.recipeIdFrom, userId, chatId, bot, api);
        stateManager.put(RecipeView{.storageIds = state.storageIdsFrom,
                                    .recipeId = state.recipeIdFrom,
                                    .fromStorage = state.fromStorage,
                                    .pageNo = state.pageNo});
        bot.answerCallbackQuery(cq.id);
        return;
    }
    if (data[0] == 'i') {
        auto newIngredientIdStr =
            data.substr(1, data.size()); // Here we got all selected storages and new one as last in string
        auto ingredientIdToRemove = utils::parseSafe<api::IngredientId>(newIngredientIdStr);
        // Remove ingredient which was chosen
        if (ingredientIdToRemove) {
            for (auto ingredientId = state.ingredientIdsInList.begin(); ingredientId < state.ingredientIdsInList.end();
                 ingredientId++) {
                if (*ingredientId == *ingredientIdToRemove) {
                    state.ingredientIdsInList.erase(ingredientId);
                }
            }
        }

        renderEditedShoppingListCreation(state.ingredientIdsInList, userId, chatId, bot, api);
    }
}
} // namespace cookcookhnya::handlers::shopping_list
