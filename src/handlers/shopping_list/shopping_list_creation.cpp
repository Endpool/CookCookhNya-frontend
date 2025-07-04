#include "handlers/shopping_list/shopping_list_creation.hpp"

#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/recipes_suggestion/recipe_view_render.hpp"
#include "render/shopping_list/shopping_list_creation_render.hpp"

#include <sstream>
#include <string>
#include <vector>

namespace cookcookhnya::handlers::shopping_list_creation {
using namespace render::shopping_list_creation;
using namespace render::recipe_view;

void handleProductListSubmission(
    ShoppingListCreation& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    std::string data = cq.data;
    std::stringstream temp; // Convert string to int
    const std::vector<api::IngredientId> ingredientIds;
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    if (data == "BackFromShoppingList") {
        renderRecipeViewAfterAddingStorage(state.storageIdsFrom, state.recipeIdFrom, userId, chatId, bot, api);
        stateManager.put(RecipeView{.storageIds = state.storageIdsFrom, .recipeId = state.recipeIdFrom});
        bot.answerCallbackQuery(cq.id);
        return;
    }
    if (data == "AcceptShoppingList") {
        // Put ingredients in list
        auto shoppingApi = api.getShoppingListApi();
        shoppingApi.put(userId, state.ingredientIdsInList);

        // Return to previous state
        renderRecipeViewAfterAddingStorage(state.storageIdsFrom, state.recipeIdFrom, userId, chatId, bot, api);
        stateManager.put(RecipeView{.storageIds = state.storageIdsFrom, .recipeId = state.recipeIdFrom});
        bot.answerCallbackQuery(cq.id);
        return;
    }
    if (data[0] == 'i') {
        auto newIngredientIdStr =
            data.substr(1, data.size()); // Here we got all selected storages and new one as last in string
        api::IngredientId ingredientIdToRemove = 0;
        temp << newIngredientIdStr;
        temp >> ingredientIdToRemove;
        // Remove ingredient which was chosen
        for (auto ingredientId = state.ingredientIdsInList.begin(); ingredientId < state.ingredientIdsInList.end();
             ingredientId++) {
            if (*ingredientId == ingredientIdToRemove) {
                state.ingredientIdsInList.erase(ingredientId);
            }
        }
        renderEditedShoppingListCreation(state.ingredientIdsInList, userId, chatId, bot, api);
    }
}
} // namespace cookcookhnya::handlers::shopping_list_creation
