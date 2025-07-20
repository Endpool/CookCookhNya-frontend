#include "delete.hpp"

#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"
#include "render/storage/ingredients/delete.hpp"
#include "render/storage/ingredients/view.hpp"
#include "states.hpp"
#include "utils/parsing.hpp"
#include <algorithm>

namespace cookcookhnya::handlers::storage::ingredients {

using namespace render::storage::ingredients;
using namespace std::views;

void handleStorageIngredientsDeletionCQ(
    StorageIngredientsDeletion& state, CallbackQueryRef cq, BotRef& bot, SMRef stateManager, ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    auto userId = cq.from->id;
    auto chatId = cq.message->chat->id;

    if (cq.data == "delete") {
        for (const auto& ing : state.selectedIngredients) {
            api.getIngredientsApi().deleteFromStorage(userId, state.storageId, ing.id);
            state.selectedIngredients.erase(
                std::ranges::find(state.selectedIngredients, ing.id, &api::models::ingredient::Ingredient::id));
            state.storageIngredients.erase(
                std::ranges::find(state.storageIngredients, ing.id, &api::models::ingredient::Ingredient::id));
        }
        auto ingredients = api.getIngredientsApi().getStorageIngredients(userId, state.storageId);
        auto newState = StorageIngredientsList{state.storageId, ingredients | as_rvalue, ""};
        renderIngredientsListSearch(newState, userId, chatId, bot);
        stateManager.put(newState);
        return;
    }
    if (cq.data == "put_to_shop") {
        api.getShoppingListApi().put(
            userId,
            state.selectedIngredients | std::views::transform([](const api::models::ingredient::Ingredient& obj) {
                return obj.id;
            }) | std::ranges::to<std::vector>());
        state.addedToShopList = true;
        renderStorageIngredientsDeletion(state, userId, chatId, bot);
        stateManager.put(StorageIngredientsDeletion{
            state.storageId, state.selectedIngredients, state.storageIngredients, state.addedToShopList, state.pageNo});
        return;
    }
    if (cq.data == "back") {
        auto ingredients = api.getIngredientsApi().getStorageIngredients(userId, state.storageId);
        auto newState = StorageIngredientsList{state.storageId, ingredients | as_rvalue, ""};
        renderIngredientsListSearch(newState, userId, chatId, bot);
        stateManager.put(newState);
        return;
    }
    if (cq.data == "prev") {
        state.pageNo -= 1;
        renderStorageIngredientsDeletion(state, userId, chatId, bot);
        stateManager.put(StorageIngredientsDeletion{
            state.storageId, state.selectedIngredients, state.storageIngredients, state.addedToShopList, state.pageNo});
        return;
    }
    if (cq.data == "next") {
        state.pageNo += 1;
        renderStorageIngredientsDeletion(state, userId, chatId, bot);
        stateManager.put(StorageIngredientsDeletion{
            state.storageId, state.selectedIngredients, state.storageIngredients, state.addedToShopList, state.pageNo});
        return;
    }
    if (cq.data != "dont_handle") {
        if (auto id = utils::parseSafe<api::IngredientId>(cq.data.substr(1))) {
            if (cq.data[0] == '+') {
                auto it = std::ranges::find(state.selectedIngredients, *id, &api::models::ingredient::Ingredient::id);
                state.selectedIngredients.erase(it);
            } else if (cq.data[0] == '-') {
                auto ingredientDetails = api.getIngredientsApi().get(userId, *id);
                state.selectedIngredients.push_back({.id = *id, .name = ingredientDetails.name});
            }
            renderStorageIngredientsDeletion(state, userId, chatId, bot);
            return;
        }
    }
}
} // namespace cookcookhnya::handlers::storage::ingredients
