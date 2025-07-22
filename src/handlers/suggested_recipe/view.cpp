#include "view.hpp"

#include "backend/api/api.hpp"
#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"
#include "handlers/common.hpp"
#include "render/recipes_suggestions/view.hpp"
#include "render/shopping_list/create.hpp"
#include "render/suggested_recipe/add_storage.hpp"
#include "states.hpp"

#include <string>
#include <utility>
#include <vector>

namespace cookcookhnya::handlers::recipe {

using namespace render::recipes_suggestions;
using namespace render::shopping_list;
using namespace render::recipe;
using namespace api::models::ingredient;
using IngredientAvailability = states::RecipeView::IngredientAvailability;
using AvailabilityType = states::RecipeView::AvailabilityType;

void handleRecipeViewCQ(RecipeView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::ApiClientRef api) {
    const std::string data = cq.data;
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    if (data == "start_cooking") {
        // TODO: add state of begginig of cooking
        return;
    }

    if (data == "shopping_list") {
        std::vector<Ingredient> selectedIngredients;
        std::vector<Ingredient> allIngredients;
        for (const auto& av : state.availability) {
            if (av.available == AvailabilityType::NOT_AVAILABLE) {
                selectedIngredients.push_back({.id = av.ingredient.id, .name = av.ingredient.name});
            }
            allIngredients.push_back({.id = av.ingredient.id, .name = av.ingredient.name});
        }
        renderShoppingListCreation(selectedIngredients, allIngredients, userId, chatId, bot);
        stateManager.put(ShoppingListCreation{
            .prevState = std::move(state),
            .selectedIngredients = selectedIngredients,
            .allIngredients = allIngredients,
        });
        bot.answerCallbackQuery(cq.id);
        return;
    }

    if (data == "back_from_recipe_view") {
        renderRecipesSuggestion(state.prevState.selectedStorages, state.prevState.pageNo, userId, chatId, bot, api);
        stateManager.put(auto{std::move(state.prevState)});
        bot.answerCallbackQuery(cq.id);
        return;
    }

    if (data == "add_storages") {
        renderStoragesSuggestion(state.availability,
                                 state.prevState.selectedStorages,
                                 state.addedStorages,
                                 state.recipeId,
                                 userId,
                                 chatId,
                                 bot,
                                 api);
        stateManager.put(RecipeStorageAddition{.prevState = std::move(state)});
        return;
    }
}

} // namespace cookcookhnya::handlers::recipe
