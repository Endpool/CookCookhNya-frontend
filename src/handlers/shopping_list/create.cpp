#include "create.hpp"

#include "backend/api/api.hpp"
#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"
#include "handlers/common.hpp"
#include "render/shopping_list/create.hpp"
#include "render/suggested_recipe/view.hpp"
#include "utils/parsing.hpp"

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

namespace cookcookhnya::handlers::shopping_list {

using namespace render::shopping_list;
using namespace render::suggested_recipe;

void handleShoppingListCreationCQ(
    ShoppingListCreation& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::ApiClientRef api) {
    const std::string& data = cq.data;
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    if (data == "back") {
        renderRecipeView(state.prevState.availability, state.prevState.recipeId, userId, chatId, bot, api);
        stateManager.put(auto{std::move(state.prevState)});
        bot.answerCallbackQuery(cq.id);
        return;
    }

    if (data == "confirm") {
        auto shoppingApi = api.getShoppingListApi();
        std::vector<api::IngredientId> putIds;
        putIds.reserve(state.selectedIngredients.size());
        for (const auto& ingredient : state.selectedIngredients) {
            putIds.push_back(ingredient.id);
        }
        shoppingApi.put(userId, putIds);
        renderRecipeView(state.prevState.availability, state.prevState.recipeId, userId, chatId, bot, api);
        stateManager.put(auto{std::move(state.prevState)});
        bot.answerCallbackQuery(cq.id);
        return;
    }

    if (data[0] == '+') {
        auto newIngredientIdStr = data.substr(1, data.size());
        auto newIngredientId = utils::parseSafe<api::IngredientId>(newIngredientIdStr);
        if (newIngredientId) {
            auto ingredient = api.getIngredientsApi().get(userId, *newIngredientId);
            state.selectedIngredients.push_back(ingredient);
        }
        renderShoppingListCreation(state.selectedIngredients, state.allIngredients, userId, chatId, bot);
        return;
    }

    if (data[0] == '-') {
        auto newIngredientIdStr = data.substr(1, data.size());
        auto newIngredientId = utils::parseSafe<api::IngredientId>(newIngredientIdStr);
        if (newIngredientId) {
            state.selectedIngredients.erase(std::ranges::find(
                state.selectedIngredients, *newIngredientId, &api::models::ingredient::Ingredient::id));
        }
        renderShoppingListCreation(state.selectedIngredients, state.allIngredients, userId, chatId, bot);
        return;
    }
}
} // namespace cookcookhnya::handlers::shopping_list
