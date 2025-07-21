#include "create.hpp"

#include "backend/api/recipes.hpp"
#include "backend/models/recipe.hpp"
#include "handlers/common.hpp"
#include "render/personal_account/recipe/view.hpp"
#include "render/personal_account/recipes_list/view.hpp"

namespace cookcookhnya::handlers::personal_account::recipes {

using namespace render::personal_account::recipes;

void handleCreateCustomRecipeMsg(
    CreateCustomRecipe& /*unused*/, MessageRef m, BotRef bot, SMRef stateManager, api::RecipesApiRef recipeApi) {
    // Init with no ingredients and link. My suggestion: to use link as author's alias
    auto recipeId = recipeApi.create(
        m.from->id, api::models::recipe::RecipeCreateBody{.name = m.text, .ingredients = {}, .link = ""});

    renderCustomRecipe(false, m.from->id, m.chat->id, recipeId, bot, recipeApi);
    // If it went from creation then as user will return
    // from RecipeView to RecipesList on 1st page
    stateManager.put(RecipeCustomView{.recipeId = recipeId, .pageNo = 0, .ingredients = {}});
};

void handleCreateCustomRecipeCQ(
    CreateCustomRecipe& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::RecipesApiRef recipeApi) {
    bot.answerCallbackQuery(cq.id);

    if (cq.data == "cancel_recipe_creation") {
        renderCustomRecipesList(state.pageNo, cq.from->id, cq.message->chat->id, bot, recipeApi);
        // If it went from creation then as user will return from
        // RecipeView to RecipesList on page from which they entered in this state;
        stateManager.put(CustomRecipesList{.pageNo = state.pageNo});
    }
};

} // namespace cookcookhnya::handlers::personal_account::recipes
