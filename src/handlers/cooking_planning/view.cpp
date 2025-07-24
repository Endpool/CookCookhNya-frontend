#include "view.hpp"

#include "backend/api/api.hpp"
#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"
#include "handlers/common.hpp"
#include "render/cooking/ingredients_spending.hpp"
#include "render/cooking_planning/add_storage.hpp"
#include "render/recipes_suggestions/view.hpp"
#include "render/shopping_list/create.hpp"
#include "states.hpp"

#include <optional>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

namespace cookcookhnya::handlers::cooking_planning {

using namespace render::recipes_suggestions;
using namespace render::shopping_list;
using namespace render::cooking_planning;
using namespace render::cooking;

using namespace api::models::ingredient;
using IngredientAvailability = states::CookingPlanning::IngredientAvailability;
using AvailabilityType = states::CookingPlanning::AvailabilityType;
using states::helpers::SelectableIngredient;

using namespace std::views;
using std::ranges::to;

void handleCookingPlanningCQ(
    CookingPlanning& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    const std::string data = cq.data;
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    if (data == "start_cooking") {
        std::optional<api::StorageId> theOnlyStorage;
        if (state.prevState.selectedStorages.size() == 1)
            theOnlyStorage = state.prevState.selectedStorages.front().id;
        auto ingredients =
            state.availability |
            transform([](const auto& ia) { return SelectableIngredient{{ia.ingredient.id, ia.ingredient.name}}; }) |
            to<std::vector>();
        renderIngredientsSpending(ingredients, theOnlyStorage.has_value(), userId, chatId, bot);
        CookingIngredientsSpending newState{
            .prevState = std::move(state), .storageId = theOnlyStorage, .ingredients = std::move(ingredients)};
        stateManager.put(std::move(newState));
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
        return;
    }

    if (data == "back_from_recipe_view") {
        renderRecipesSuggestion(state.prevState.selectedStorages, state.prevState.pageNo, userId, chatId, bot, api);
        stateManager.put(auto{std::move(state.prevState)});
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
        stateManager.put(CookingPlanningStorageAddition{.prevState = std::move(state)});
        return;
    }
}

} // namespace cookcookhnya::handlers::cooking_planning
