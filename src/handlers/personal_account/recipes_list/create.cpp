#include "create.hpp"

#include "handlers/common.hpp"
#include "render/personal_account/recipes_list/recipe/view.hpp"
#include "render/personal_account/recipes_list/view.hpp"

namespace cookcookhnya::handlers::personal_account::recipes {

using namespace render::personal_account::recipes;

void createRecipe(CreateCustomRecipe& state, MessageRef m, BotRef bot, SMRef stateManager, RecipesApiRef recipeApi) {
    // UNCOMMENT WHEN BACKEND IS READY
    // state.recipeId = recipeApi.create(m.from->id, api::models::recipe::RecipeCreateBody{m.text});

    renderCustomRecipe(false, m.from->id, m.chat->id, state.recipeId, bot, recipeApi);
    stateManager.put(
        RecipeCustomView{.recipeId = state.recipeId, .pageNo = 0}); // If it went from creation then as user will return
                                                                    // from RecipeView to RecipesList on 1st page
};

void cancelRecipeCreation(
    CreateCustomRecipe& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, RecipesApiRef recipeApi) {
    bot.answerCallbackQuery(cq.id);
    if (cq.data == "cancel_recipe_creation") {
        renderCustomRecipesList(state.pageNo, cq.from->id, cq.message->chat->id, bot, recipeApi);
        stateManager.put(
            CustomRecipesList{.pageNo = state.pageNo}); // If it went from creation then as user will return from
                                                        // RecipeView to RecipesList on 1st page);
    }
};

} // namespace cookcookhnya::handlers::personal_account::recipes
