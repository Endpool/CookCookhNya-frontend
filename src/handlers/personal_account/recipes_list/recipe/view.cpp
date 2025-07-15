#include "view.hpp"

#include "handlers/common.hpp"
#include "render/personal_account/recipes_list/recipe/search_ingredients.hpp"
#include "render/personal_account/recipes_list/view.hpp"
#include <string>

namespace cookcookhnya::handlers::personal_account::recipes {

using namespace render::personal_account::recipes;
using namespace render::recipe::ingredients;

void handleRecipeCustomViewCQ(
    RecipeCustomView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, RecipesApiRef recipesApi) {
    const std::string data = cq.data;
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    if (data == "back") {
        renderCustomRecipesList(state.pageNo, userId, chatId, bot, recipesApi);
        stateManager.put(CustomRecipesList{.pageNo = state.pageNo});
        return;
    }

    if (data == "change") {
        // Reuse search render
        renderStorageIngredientsSearch(chatId, userId, bot);
        stateManager.put(CustomRecipeIngredientsSearch{
            .recipeId = state.recipeId, .shownIngredients = {}, .totalFound = 0, .pageNo = 0});
        return;
    }

    if (data == "delete") {
        recipesApi.delete_(userId, state.recipeId);
        renderCustomRecipesList(state.pageNo, userId, chatId, bot, recipesApi);
        stateManager.put(CustomRecipesList{.pageNo = state.pageNo});
        return;
    }

    if (data == "publish") { // Should also create backend endpoint to track status of publish
        recipesApi.publishRecipe(userId, state.recipeId);
        return;
    }
}

} // namespace cookcookhnya::handlers::personal_account::recipes
