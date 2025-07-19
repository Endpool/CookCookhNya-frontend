#include "publication_history.hpp"

#include "render/personal_account/recipe/view.hpp"

namespace cookcookhnya::handlers::personal_account::publication_history {

using namespace render::personal_account;
using namespace render::personal_account::recipes;

void handleCustomRecipePublicationHistoryCQ(CustomRecipePublicationHistory& state,
                                            CallbackQueryRef cq,
                                            BotRef bot,
                                            SMRef stateManager,
                                            RecipesApiRef recipesApi) {
    const std::string data = cq.data;
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    if (data == "back") {
        auto ingredients = renderCustomRecipe(true, userId, chatId, state.recipeId, bot, recipesApi);
        stateManager.put(
            RecipeCustomView{.recipeId = state.recipeId, .pageNo = state.pageNo, .ingredients = ingredients});
        return;
    }

    if (data == "confirm") {
        recipesApi.publishCustom(userId, state.recipeId);
        // As published return back
        auto ingredients = renderCustomRecipe(true, userId, chatId, state.recipeId, bot, recipesApi);
        stateManager.put(
            RecipeCustomView{.recipeId = state.recipeId, .pageNo = state.pageNo, .ingredients = ingredients});
        return;
    }
}

} // namespace cookcookhnya::handlers::personal_account::publication_history
