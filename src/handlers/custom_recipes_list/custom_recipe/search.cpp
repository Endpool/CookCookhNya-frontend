#include "search.hpp"

#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"
#include "handlers/common.hpp"
#include "message_tracker.hpp"
#include "render/custom_recipes_list/custom_recipe/view.hpp"
#include "render/storage_view/ingredients/search.hpp" // REUSED THE SAME RENDER!!!

#include "utils.hpp"

#include <algorithm>
#include <cstddef>
#include <functional>
#include <utility>

namespace cookcookhnya::handlers::recipe::ingredients {

using namespace api::models::ingredient;
using namespace render::storage::ingredients;
using namespace render::custom_recipe_view;

void customRecipeIngredientsSearchButtonCallback(
    CustomRecipeIngredientsSearch& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    auto ingredientsApi = api.getIngredientsApi();

    bot.answerCallbackQuery(cq.id);
    const auto userId = cq.from->id;
    const auto chatId = cq.message->chat->id;
    if (cq.data == "back") {
        renderCustomRecipe(false, userId, chatId, state.recipeId, bot, api);
        stateManager.put(RecipeCustomView{.recipeId = state.recipeId, .pageNo = state.pageNo});
        return;
    }

    auto mIngredient = utils::parseSafe<api::IngredientId>(cq.data);
    if (!mIngredient)
        return;
    auto it = std::ranges::find(state.shownIngredients, *mIngredient, &IngredientSearchItem::id);
    if (it == state.shownIngredients.end())
        return;
    if (it->available) {
        // UNCOMMENT WHEN BACKEND IS READY
        // ingredientsApi.deleteFromRecipe(userId, state.recipeId, *mIngredient);
    } else {
        // UNCOMMENT WHEN BACKEND IS READY
        // ingredientsApi.putToRecipe(userId, state.recipeId, *mIngredient);
    }
    it->available = !it->available;

    if (auto mMessageId = message::getMessageId(userId))
        renderStorageIngredientsSearchEdit(state.shownIngredients, state.pageNo, 1, *mMessageId, chatId, bot);
}

void customRecipeIngredientsSearchInlineQueryCallback(CustomRecipeIngredientsSearch& state,
                                                      InlineQueryRef iq,
                                                      BotRef bot,
                                                      IngredientsApiRef api) {
    if (!iq.query.empty()) {
        const auto userId = iq.from->id;
        const std::size_t count = 100;
        // CHECK BACKEND WHEN IT'S READY
        auto response = api.searchForRecipe(userId, iq.query, state.recipeId, count, 0);
        if (response.found != state.totalFound || !std::ranges::equal(response.page,
                                                                      state.shownIngredients,
                                                                      std::ranges::equal_to{},
                                                                      &IngredientSearchItem::id,
                                                                      &IngredientSearchItem::id)) {
            state.shownIngredients = std::move(response.page);
            state.totalFound = response.found;
            if (auto mMessageId = message::getMessageId(userId))
                renderStorageIngredientsSearchEdit(state.shownIngredients, state.pageNo, 1, *mMessageId, userId, bot);
        }
    }
    // Cache is not disabled on Windows and Linux desktops. Works on Android and Web
    bot.answerInlineQuery(iq.id, {}, 0);
}

} // namespace cookcookhnya::handlers::recipe::ingredients
