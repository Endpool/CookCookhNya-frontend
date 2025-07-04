#include "recipe_view.hpp"

#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/recipes_suggestion/recipe_view_render.hpp"
#include "render/recipes_suggestion/recipes_suggestion_render.hpp"
#include "render/recipes_suggestion/shopping_list_creation_render.hpp"

#include <sstream>
#include <string>

namespace cookcookhnya::handlers::recipe_view {

using namespace render::recipe_view;
using render::recipes_suggestion::editRecipesSuggestion;
using render::shopping_list_creation::renderShoppingListCreation;

void handleRecipeView(RecipeView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    std::stringstream temp; // Convert string to int
    std::string data = cq.data;
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    if (data == "startCooking") {
        // TODO: add state of begginig of cooking
        return;
    }
    if (data == "makeProductList") {
        auto ingredientsInList = renderShoppingListCreation(state.storageIds, state.recipeId, userId, chatId, bot, api);
        stateManager.put(ShoppingListCreation{.storageIdsFrom = state.storageIds,
                                              .recipeIdFrom = state.recipeId,
                                              .ingredientIdsInList = ingredientsInList,
                                              .fromStorage = state.fromStorage,
                                              .pageNo = state.pageNo});
        bot.answerCallbackQuery(cq.id);
        return;
    }
    if (data == "backFromRecipeView") {
        editRecipesSuggestion(state.storageIds, 0, userId, chatId, bot, api);
        stateManager.put(
            SuggestedRecipeList{.pageNo = 0, .storageIds = state.storageIds, .fromStorage = state.fromStorage});
        bot.answerCallbackQuery(cq.id);
        return;
    }
    if (data == "BackFromAddingStorages") {
        renderRecipeViewAfterAddingStorage(state.storageIds, state.recipeId, userId, chatId, bot, api);
        return;
    }
    if (data[0] == '?') {
        renderStorageSuggestion(state.storageIds,
                                state.recipeId,
                                userId,
                                chatId,
                                bot,
                                api); // dangerous to ge message id like that?
        // What? (by Maxim Fomin)
        return;
    }
    if (data[0] == '+') {
        auto newStorageIdStr =
            data.substr(1, data.size()); // Here we got all selected storages and new one as last in string
        api::StorageId newStorageId = 0;
        temp << newStorageIdStr;
        temp >> newStorageId;
        state.storageIds.push_back(newStorageId);
        renderStorageSuggestion(state.storageIds, state.recipeId, userId, chatId, bot, api);
    }
}

} // namespace cookcookhnya::handlers::recipe_view
