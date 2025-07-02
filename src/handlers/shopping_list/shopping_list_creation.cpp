#include "handlers/shopping_list/shopping_list_creation.hpp"
#include "handlers/common.hpp"
#include "render/recipes_suggestion/recipe_view_render.hpp"
#include "render/shopping_list/shopping_list_creation_render.hpp"

namespace cookcookhnya::handlers::shopping_list_creation {
using namespace render::shopping_list_creation;
using namespace render::recipe_view;

void handleProductListSubmission(
    ShoppingListCreation& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    std::string data = cq.data;
    std::stringstream temp; // Convert string to int
    auto messageId = cq.message->messageId;

    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    if (data == "BackFromShoppingList") {
        renderRecipeView(state.storageIdsFrom, state.recipeIdFrom, userId, chatId, bot, api);
        stateManager.put(RecipeView{.storageIds = state.storageIdsFrom, .recipeId = state.recipeIdFrom});
        return;
    }
    if (data[0] == 'i') {
        auto newIngredientIdStr =
            data.substr(1, data.size()); // Here we got all selected storages and new one as last in string
        api::IngredientId ingredientIdToRemove = 0;
        temp << newIngredientIdStr;
        temp >> ingredientIdToRemove;
        state.ingredientIdsInList.push_back(ingredientIdToRemove);
        renderEditedShoppingListCreation(state.ingredientIdsInList, ingredientIdToRemove, chatId, messageId, bot, api);
    }
}
} // namespace cookcookhnya::handlers::shopping_list_creation
