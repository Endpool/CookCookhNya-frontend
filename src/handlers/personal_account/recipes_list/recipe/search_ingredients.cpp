#include "search_ingredients.hpp"

#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"
#include "handlers/common.hpp"
#include "message_tracker.hpp"
#include "render/personal_account/recipes_list/recipe/search_ingredients.hpp"
#include "render/personal_account/recipes_list/recipe/view.hpp"
#include "tg_types.hpp"
#include "utils/parsing.hpp"

#include <algorithm>
#include <cstddef>
#include <functional>
#include <utility>
#include <vector>

namespace cookcookhnya::handlers::personal_account::recipes {

using namespace api::models::ingredient;
using namespace render::recipe::ingredients;
using namespace render::personal_account::recipes;

// Global vars
const size_t numOfIngredientsOnPage = 5;
const size_t threshhold = 70;

namespace {
void updateSearch(CustomRecipeIngredientsSearch& state, BotRef bot, tg_types::UserId userId, IngredientsApiRef api) {

    auto response = api.searchForRecipe(userId,
                                        state.recipeId,
                                        state.inlineQuery,
                                        threshhold,
                                        numOfIngredientsOnPage,
                                        state.pageNo * numOfIngredientsOnPage);
    if (response.found != state.totalFound || !std::ranges::equal(response.page,
                                                                  state.searchItems,
                                                                  std::ranges::equal_to{},
                                                                  &IngredientSearchForRecipeItem::id,
                                                                  &IngredientSearchForRecipeItem::id)) {
        state.searchItems = std::move(response.page);
        state.totalFound = response.found;
        if (auto mMessageId = message::getMessageId(userId))
            renderRecipeIngredientsSearch(state, numOfIngredientsOnPage, userId, userId, bot);
    }
}
} // namespace

void handleCustomRecipeIngredientsSearchCQ(
    CustomRecipeIngredientsSearch& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    const auto userId = cq.from->id;
    const auto chatId = cq.message->chat->id;

    if (cq.data == "back") {
        renderCustomRecipe(true, userId, chatId, state.recipeId, bot, api);
        std::vector<api::models::ingredient::Ingredient> ingredient;
        ingredient.assign(state.recipeIngredients.getAll().begin(), state.recipeIngredients.getAll().end());
        stateManager.put(RecipeCustomView{.recipeId = state.recipeId, .pageNo = 0, .ingredients = ingredient});
        return;
    }
    if (cq.data == "prev") {
        state.pageNo -= 1;
        updateSearch(state, bot, userId, api);
        return;
    }

    if (cq.data == "next") {
        state.pageNo += 1;
        updateSearch(state, bot, userId, api);
        return;
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
    const size_t numOfIngredientsOnPage = 5;
    state.inlineQuery = iq.query;
    const auto userId = iq.from->id;
    if (iq.query.empty()) {
        state.searchItems.clear();
        // When query is empty then search shouldn't happen
        state.totalFound = 0;
        renderRecipeIngredientsSearch(state, numOfIngredientsOnPage, userId, userId, bot);
    } else {
        updateSearch(state, bot, userId, api);
    }
    // Cache is not disabled on Windows and Linux desktops. Works on Android and Web
    bot.answerInlineQuery(iq.id, {}, 0);
}

} // namespace cookcookhnya::handlers::personal_account::recipes
