#include "create.hpp"

#include "backend/models/recipe.hpp"
#include "handlers/common.hpp"
#include "render/personal_account/recipes_list/recipe/view.hpp"
#include "render/personal_account/recipes_list/view.hpp"

namespace cookcookhnya::handlers::personal_account::recipes {

using namespace render::personal_account::recipes;

void handleCreateCustomRecipeMsg(
    CreateCustomRecipe& state, MessageRef m, BotRef bot, SMRef stateManager, RecipesApiRef recipeApi) {

    // Init with no ingredients and link. My suggestion: to use link as author's alias
    state.recipeId = recipeApi.create(
        m.from->id, api::models::recipe::RecipeCreateBody{.name = m.text, .ingredients = {}, .link = ""}); //

    renderCustomRecipe(false, m.from->id, m.chat->id, state.recipeId, bot, recipeApi);
    stateManager.put(RecipeCustomView{.recipeId = state.recipeId,
                                      .pageNo = 0,
                                      .ingredients = {}}); // If it went from creation then as user will return
                                                           // from RecipeView to RecipesList on 1st page
};

void handleCreateCustomRecipeCQ(
    CreateCustomRecipe& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, RecipesApiRef recipeApi) {
    bot.answerCallbackQuery(cq.id);
    if (cq.data == "cancel_recipe_creation") {
        renderCustomRecipesList(state.pageNo, cq.from->id, cq.message->chat->id, bot, recipeApi);
        stateManager.put(CustomRecipesList{
            .pageNo = state.pageNo}); // If it went from creation then as user will return from
                                      // RecipeView to RecipesList on page from which they entered in this state;
    }
};

} // namespace cookcookhnya::handlers::personal_account::recipes
