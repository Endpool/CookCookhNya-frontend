#include "view.hpp"

#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"
#include "handlers/common.hpp"
#include "render/recipe/add_storage.hpp"
#include "render/recipes_suggestions/view.hpp"
#include "render/shopping_list/create.hpp"

#include <string>

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
    if (data == "shopping_list") {
        std::vector<api::models::ingredient::Ingredient> selectedIngredients;
        for (const auto& infoPair : state.availability) {
            if (infoPair.second.available == utils::AvailabiltiyType::not_available) {
                selectedIngredients.push_back({.id = infoPair.first.id, .name = infoPair.first.name});
            }
        }
        renderShoppingListCreation(selectedIngredients, userId, chatId, bot);
        stateManager.put(ShoppingListCreation{.storages = state.storages,
                                              .availability = state.availability,
                                              .recipeId = state.recipeId,
                                              .selectedIngredients = selectedIngredients,
                                              .fromStorage = state.fromStorage,
                                              .pageNo = state.pageNo});
        bot.answerCallbackQuery(cq.id);
        return;
    }
    if (data == "back_from_recipe_view") {
        renderRecipesSuggestion(state.storages, state.pageNo, userId, chatId, bot, api);
        stateManager.put(
            SuggestedRecipeList{.pageNo = state.pageNo, .storages = state.storages, .fromStorage = state.fromStorage});
        bot.answerCallbackQuery(cq.id);
        return;
    }

    if (data == "add_storages") {
        renderStoragesSuggestion(state.availability, state.storages, state.recipeId, userId, chatId, bot, api);
        stateManager.put(RecipeStorageAddition{.storages = state.storages,
                                               .availability = state.availability,
                                               .recipeId = state.recipeId,
                                               .fromStorage = state.fromStorage,
                                               .pageNo = state.pageNo});
        return;
    }
}

} // namespace cookcookhnya::handlers::recipe
