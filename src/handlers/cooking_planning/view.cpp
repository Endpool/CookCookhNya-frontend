#include "view.hpp"

#include "backend/api/api.hpp"
#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"
#include "backend/models/storage.hpp"
#include "handlers/common.hpp"
#include "render/cooking/ingredients_spending.hpp"
#include "render/cooking_planning/add_storage.hpp"
#include "render/main_menu/view.hpp"
#include "render/recipes_search/view.hpp"
#include "render/recipes_suggestions/view.hpp"
#include "render/shopping_list/create.hpp"
#include "states.hpp"

#include <functional>
#include <optional>
#include <ranges>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace cookcookhnya::handlers::cooking_planning {

using namespace render::recipes_suggestions;
using namespace render::shopping_list;
using namespace render::cooking_planning;
using namespace render::cooking;
using namespace render::recipes_search;

using namespace api::models::ingredient;
using namespace api::models::storage;
using IngredientAvailability = states::CookingPlanning::IngredientAvailability;
using AvailabilityType = states::CookingPlanning::AvailabilityType;
using states::helpers::SelectableIngredient;

using namespace std::views;
using std::ranges::to;

namespace {

std::optional<api::StorageId> getTheOnlyStorage(const CookingPlanning& state) {
    // what a pattern matching hell (c) Team lead
    if (const auto* prevState = std::get_if<SuggestedRecipesList>(&state.prevState)) {
        if (const auto* prevPrevState = std::get_if<StorageView>(&prevState->prevState))
            return prevPrevState->storageId;
        if (const auto* prevPrevState = std::get_if<SuggestedRecipesList::FromMainMenuData>(&prevState->prevState))
            return prevPrevState->second.id;
        if (const auto* prevPrevState = std::get_if<StoragesSelection>(&prevState->prevState))
            if (prevPrevState->selectedStorages.size() == 1)
                return prevPrevState->selectedStorages.front().id;
    } else if (const auto* prevState = std::get_if<CookingPlanning::FromRecipeViewData>(&state.prevState)) {
        if (prevState->second.size() == 1)
            return prevState->second[0].id;
        return {};
    }
    return {};
}

} // namespace

void handleCookingPlanningCQ(
    CookingPlanning& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    const std::string data = cq.data;
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    if (data == "start_cooking") {
        auto ingredients =
            state.availability |
            transform([](const auto& ia) { return SelectableIngredient{{ia.ingredient.id, ia.ingredient.name}}; }) |
            to<std::vector>();
        const std::optional<api::StorageId> theOnlyStorage = getTheOnlyStorage(state);
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

    if (data == "back") {
        if (auto* prevState = std::get_if<SuggestedRecipesList>(&state.prevState)) {
            renderRecipesSuggestion(prevState->getStorageIds(), prevState->pageNo, userId, chatId, bot, api);
            stateManager.put(auto{std::move(*prevState)});
        } else if (auto* prevState = std::get_if<CookingPlanning::FromRecipeViewData>(&state.prevState)) {
            if (auto& mSearchState = prevState->first.prevState) {
                renderRecipesSearch(mSearchState->pagination, mSearchState->page, userId, chatId, bot);
                stateManager.put(auto{std::move(*mSearchState)});
            } else {
                render::main_menu::renderMainMenu(true, std::nullopt, userId, chatId, bot, api);
                stateManager.put(MainMenu{});
            }
        }
        return;
    }

    if (data == "add_storages") {
        using StoragesList = std::vector<StorageSummary>;
        auto selectedStorages = state.getStorages();
        const StoragesList* selectedStoragesPtr = nullptr;
        // very optimized decision! (no)
        if (auto* storagesVal = std::get_if<StoragesList>(&selectedStorages))
            selectedStoragesPtr = storagesVal;
        else if (auto* storagesRef = std::get_if<std::reference_wrapper<StoragesList>>(&selectedStorages))
            selectedStoragesPtr = &storagesRef->get();
        else
            return;

        renderStoragesSuggestion(
            state.availability, *selectedStoragesPtr, state.addedStorages, state.recipeId, userId, chatId, bot, api);
        stateManager.put(CookingPlanningStorageAddition{.prevState = std::move(state)});
        return;
    }
}

} // namespace cookcookhnya::handlers::cooking_planning
