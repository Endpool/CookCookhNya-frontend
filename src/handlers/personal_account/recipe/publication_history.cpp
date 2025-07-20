#include "publication_history.hpp"

#include "handlers/common.hpp"
#include "render/personal_account/recipe/publication_history.hpp"
#include "render/personal_account/recipe/view.hpp"
namespace cookcookhnya::handlers::personal_account::recipe::publication_history {

using namespace render::personal_account;
using namespace render::personal_account::recipes;
using namespace render::personal_account::recipe::publication_history;
void handleCustomRecipePublicationHistoryCQ(
    CustomRecipePublicationHistory& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    const std::string data = cq.data;
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    if (data == "back") {
        auto ingredientsAndRecipeName = renderCustomRecipe(true, userId, chatId, state.recipeId, bot, api);
        stateManager.put(RecipeCustomView{.recipeId = state.recipeId,
                                          .pageNo = state.pageNo,
                                          .ingredients = std::get<0>(ingredientsAndRecipeName),
                                          .recipeName = std::get<1>(ingredientsAndRecipeName)});
        return;
    }

    if (data == "confirm") {
        // Peeking (if button with this data then accepted or pending)
        api.getRecipesApi().publishCustom(userId, state.recipeId);
        bool isPeeking = true;
        renderPublicationHistory(userId, chatId, state.recipeId, state.recipeName, isPeeking, bot, api);
        stateManager.put(states::CustomRecipePublicationHistory{
            .recipeId = state.recipeId, .pageNo = state.pageNo, .recipeName = state.recipeName});
        bot.answerCallbackQuery(cq.id);
        return;
    }
}

} // namespace cookcookhnya::handlers::personal_account::recipe::publication_history
