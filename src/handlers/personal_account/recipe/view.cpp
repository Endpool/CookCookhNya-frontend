#include "view.hpp"

#include "backend/api/api.hpp"
#include "handlers/common.hpp"
#include "render/personal_account/recipe/search_ingredients.hpp"
#include "render/personal_account/recipes_list/view.hpp"
#include "states.hpp"

#include <cstddef>
#include <ranges>
#include <string>
#include <utility>

namespace cookcookhnya::handlers::personal_account::recipes {

using namespace render::personal_account::recipes;
using namespace render::recipe::ingredients;
using namespace std::views;

const std::size_t numOfIngredientsOnPage = 5;

void handleRecipeCustomViewCQ(
    RecipeCustomView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::ApiClientRef api) {
    const std::string data = cq.data;
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    if (data == "back") {
        renderCustomRecipesList(state.pageNo, userId, chatId, bot, api);
        stateManager.put(CustomRecipesList{.pageNo = state.pageNo});
        return;
    }

    if (data == "change") {
        auto newState = CustomRecipeIngredientsSearch{state.recipeId, state.ingredients | as_rvalue, ""};
        renderRecipeIngredientsSearch(newState, numOfIngredientsOnPage, userId, chatId, bot);
        stateManager.put(std::move(newState));
        return;
    }

    if (data == "delete") {
        api.getRecipesApi().delete_(userId, state.recipeId);
        // If some recipe was deleted then return to first page
        // Made to avoid bug when delete last recipe on page -> will return to the non-existent page
        renderCustomRecipesList(0, userId, chatId, bot, api);
        stateManager.put(CustomRecipesList{.pageNo = 0});
        return;
    }

    if (data == "publish") { // Should also create backend endpoint to track status of publish
        api.getRecipesApi().publishCustom(userId, state.recipeId);
        return;
    }
}

} // namespace cookcookhnya::handlers::personal_account::recipes
