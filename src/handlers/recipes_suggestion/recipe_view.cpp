#include "recipe_view.hpp"
#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/recipes_suggestion/recipe_view_render.hpp"
#include "render/recipes_suggestion/recipes_suggestion_render.hpp"
#include "render/shopping_list/shopping_list_creation_render.hpp"

namespace cookcookhnya::handlers::recipe_view {
using namespace render::recipe_view;
using render::recipes_suggestion::renderRecipesSuggestion;
using render::shopping_list_creation::renderShoppingListCreation;

void handleRecipeView(RecipeView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    std::stringstream temp; // Convert string to int
    std::string data = cq.data;
    auto messageId = cq.message->messageId;

    auto chatId = cq.message->chat->id;
    [[maybe_unused]] auto messageId = cq.message->messageId;
    auto userId = cq.from->id;

    if (data == "startCooking") {
        // TODO: add state of begginig of cooking
        return;
    }
    if (data == "makeProductList") {
        auto ingredientsInList = renderShoppingListCreation(state.storageIds, state.recipeId, userId, chatId, bot, api);
        stateManager.put(ShoppingListCreation{.storageIdsFrom = state.storageIds,
                                              .recipeIdFrom = state.recipeId,
                                              .ingredientIdsInList = ingredientsInList});
        bot.answerCallbackQuery(cq.id);
        return;
    }
    if (data == "backFromRecipeView") {
        renderRecipesSuggestion(state.storageIds, 1, userId, chatId, bot, api);
        stateManager.put(SuggestedRecipeList{.pageNo = 1,
                                             .storageIds = std::move(state.storageIds),
                                             .fromStorage = state.storageIds.size() ==
                                                            0}); // I don't know if it came from storage or no
        bot.answerCallbackQuery(cq.id);
        return;
    }
    if (data == "BackFromAddingStorages") {
        renderRecipeViewAfterAddingStorage(state.storageIds, state.recipeId, userId, chatId, messageId, bot, api);
    }
    if (data[0] == '?') {
        renderStorageSuggestion(state.storageIds, state.recipeId, userId, chatId, messageId, bot, api);

        return;
    }
    if (data[0] == '+') {
        auto newStorageIdStr =
            data.substr(1, data.size()); // Here we got all selected storages and new one as last in string
        api::StorageId newStorageId = 0;
        temp << newStorageIdStr;
        temp >> newStorageId;
        state.storageIds.push_back(newStorageId);
        renderStorageSuggestion(state.storageIds, state.recipeId, userId, chatId, messageId, bot, api);
    }
}

} // namespace cookcookhnya::handlers::recipe_view
