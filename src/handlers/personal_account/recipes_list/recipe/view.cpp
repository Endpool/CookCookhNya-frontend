#include "view.hpp"

#include "handlers/common.hpp"
#include "render/personal_account/recipes_list/recipe/search_ingredients.hpp"
#include "render/personal_account/recipes_list/view.hpp"
#include "states.hpp"
#include <string>

namespace cookcookhnya::handlers::personal_account::recipes {

using namespace render::personal_account::recipes;
using namespace render::recipe::ingredients;

void handleRecipeCustomViewCQ(
    RecipeCustomView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    const std::string data = cq.data;
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;
    const size_t numOfIngredientsOnPage = 5;
    if (data == "back") {
        renderCustomRecipesList(state.pageNo, userId, chatId, bot, api);
        stateManager.put(CustomRecipesList{.pageNo = state.pageNo});
        return;
    }

    if (data == "change") {
        stateManager.put(CustomRecipeIngredientsSearch{
            state.recipeId,
            {std::make_move_iterator(state.ingredients.begin()), std::make_move_iterator(state.ingredients.end())}});
        renderRecipeIngredientsSearch(
            std::get<CustomRecipeIngredientsSearch>(*stateManager.get()), numOfIngredientsOnPage, userId, chatId, bot);
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
