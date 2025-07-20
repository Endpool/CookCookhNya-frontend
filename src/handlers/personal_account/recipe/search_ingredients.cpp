#include "search_ingredients.hpp"

#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"
#include "handlers/common.hpp"
#include "message_tracker.hpp"
#include "render/personal_account/ingredients_list/create.hpp"
#include "render/personal_account/recipe/search_ingredients.hpp"
#include "render/personal_account/recipe/view.hpp"
#include "tg_types.hpp"
#include "utils/parsing.hpp"

#include <algorithm>
#include <cstddef>
#include <functional>
#include <optional>
#include <ranges>
#include <utility>
#include <vector>

namespace cookcookhnya::handlers::personal_account::recipes {

using namespace api::models::ingredient;
using namespace render::recipe::ingredients;
using namespace render::personal_account::recipes;
using namespace std::ranges;
using namespace std::views;
using namespace render::personal_account::ingredients;
using namespace render::suggest_custom_ingredient;
namespace {

const std::size_t numOfIngredientsOnPage = 5;
const std::size_t threshhold = 70;

void updateSearch(CustomRecipeIngredientsSearch& state,
                  bool isQueryChanged,
                  BotRef bot,
                  tg_types::UserId userId,
                  IngredientsApiRef api) {
    state.pageNo = isQueryChanged ? 0 : state.pageNo;
    auto response = api.searchForRecipe(
        userId, state.recipeId, state.query, threshhold, numOfIngredientsOnPage, state.pageNo * numOfIngredientsOnPage);
    if (response.found != state.totalFound || !std::ranges::equal(response.page,
                                                                  state.searchItems,
                                                                  std::ranges::equal_to{},
                                                                  &IngredientSearchForRecipeItem::id,
                                                                  &IngredientSearchForRecipeItem::id)) {
        state.searchItems = std::move(response.page);
        state.totalFound = response.found;
        if (auto mMessageId = message::getMessageId(userId)) {
            if (state.totalFound != 0) {
                renderRecipeIngredientsSearch(state, numOfIngredientsOnPage, userId, userId, bot);
                return;
            }
        }
    }
    if (state.totalFound == 0)
        renderSuggestIngredientCustomisation(state, userId, userId, bot);
}

} // namespace

void handleCustomRecipeIngredientsSearchCQ(
    CustomRecipeIngredientsSearch& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    const auto userId = cq.from->id;
    const auto chatId = cq.message->chat->id;

    if (cq.data == "back") {
        auto ingredientsAndName = renderCustomRecipe(true, userId, chatId, state.recipeId, bot, api);
        auto ingredients = state.recipeIngredients.getValues() | as_rvalue | to<std::vector>();
        stateManager.put(RecipeCustomView{.recipeId = state.recipeId,
                                          .pageNo = state.pageNo,
                                          .ingredients = std::move(ingredients),
                                          .recipeName = std::get<1>(ingredientsAndName)});
        return;
    }

    if (cq.data == "prev") {
        state.pageNo -= 1;
        updateSearch(state, false, bot, userId, api);
        return;
    }

    if (cq.data == "next") {
        state.pageNo += 1;
        updateSearch(state, false, bot, userId, api);
        return;
    }

    if (cq.data[0] == 'i') {
        auto ingredientName = cq.data.substr(1);
        renderCustomIngredientConfirmation(true, ingredientName, userId, chatId, bot, api);
        auto ingredients = state.recipeIngredients.getValues() | as_rvalue | to<std::vector>();
        stateManager.put(
            CustomIngredientConfirmation{ingredientName, state.recipeId, state.pageNo, ingredients, std::nullopt});
    }

    if (cq.data != "dont_handle") {
        auto mIngredient = utils::parseSafe<api::IngredientId>(cq.data);
        if (!mIngredient)
            return;
        auto it = std::ranges::find(state.searchItems, *mIngredient, &IngredientSearchForRecipeItem::id);
        if (it == state.searchItems.end())
            return;

        if (it->isInRecipe) {
            api.getIngredientsApi().deleteFromRecipe(userId, state.recipeId, *mIngredient);
            state.recipeIngredients.remove(*mIngredient);
        } else {
            api.getIngredientsApi().putToRecipe(userId, state.recipeId, *mIngredient);
            state.recipeIngredients.put({.id = it->id, .name = it->name});
        }
        it->isInRecipe = !it->isInRecipe;
        renderRecipeIngredientsSearch(state, numOfIngredientsOnPage, userId, chatId, bot);
    }
}

void handleCustomRecipeIngredientsSearchIQ(CustomRecipeIngredientsSearch& state,
                                           InlineQueryRef iq,
                                           BotRef bot,
                                           IngredientsApiRef api) {
    state.query = iq.query;
    const auto userId = iq.from->id;
    if (iq.query.empty()) {
        // When query is empty then search shouldn't happen
        state.searchItems.clear();
        state.totalFound = 0;
        renderRecipeIngredientsSearch(state, numOfIngredientsOnPage, userId, userId, bot);
    } else {
        updateSearch(state, true, bot, userId, api);
    }
    // Cache is not disabled on Windows and Linux desktops. Works on Android and Web
    bot.answerInlineQuery(iq.id, {}, 0);
}

} // namespace cookcookhnya::handlers::personal_account::recipes
