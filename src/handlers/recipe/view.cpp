#include "view.hpp"

#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/recipe/add_storage.hpp"
#include "render/recipes_suggestions/view.hpp"
#include "render/shopping_list/create.hpp"

#include <string>
#include <unordered_set>
#include <vector>

namespace cookcookhnya::handlers::recipe {

using namespace render::recipes_suggestions;
using namespace render::shopping_list;
using namespace render::recipe;

void handleRecipeViewCQ(RecipeView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    std::string data = cq.data;
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    if (data == "start_cooking") {
        // TODO: add state of begginig of cooking
        return;
    }
    if (data == "make_product_list") {
        // Next lines of code is necessary preparation for making product list. Need to re-verify products which are
        // certanly not in storage
        // Besides we exactly need ingredient Ids, as then it will be sent on backend
        const std::unordered_set<api::StorageId> storageIdsSet(state.storageIds.begin(), state.storageIds.end());
        auto recipesApi = api.getRecipesApi();

        auto ingredients = recipesApi.getIngredientsInRecipe(userId, state.recipeId).ingredients;
        std::vector<api::IngredientId> ingredientIds;
        bool isHavingIngredient = false;

        for (auto& ingredient : ingredients) { // Iterate through each ingredient
            isHavingIngredient = false;
            for (const api::StorageId storage : ingredient.inStorages) {
                // Then for this ingredient one of possible storages already selected
                if (storageIdsSet.contains(storage)) {
                    isHavingIngredient = true;
                    break; // No need to iterate further
                }
            }
            if (!isHavingIngredient) {
                ingredientIds.push_back(ingredient.id);
            }
        }
        renderShoppingListCreation(ingredientIds, userId, chatId, bot, api);
        stateManager.put(ShoppingListCreation{.storageIdsFrom = state.storageIds,
                                              .recipeIdFrom = state.recipeId,
                                              .ingredientIdsInList = ingredientIds,
                                              .fromStorage = state.fromStorage,
                                              .pageNo = state.pageNo});
        bot.answerCallbackQuery(cq.id);
        return;
    }
    if (data == "back_from_recipe_view") {
        renderRecipesSuggestion(state.storageIds, state.pageNo, userId, chatId, bot, api);
        stateManager.put(SuggestedRecipeList{
            .pageNo = state.pageNo, .storageIds = state.storageIds, .fromStorage = state.fromStorage});
        bot.answerCallbackQuery(cq.id);
        return;
    }

    if (data[0] == '?') {
        renderStoragesSuggestion(state.storageIds, state.recipeId, userId, chatId, bot, api);
        stateManager.put(RecipeStorageAddition{.storageIds = state.storageIds,
                                               .recipeId = state.recipeId,
                                               .fromStorage = state.fromStorage,
                                               .pageNo = state.pageNo});
        return;
    }
}

} // namespace cookcookhnya::handlers::recipe
