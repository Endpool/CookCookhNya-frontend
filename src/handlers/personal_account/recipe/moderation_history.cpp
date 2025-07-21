#include "moderation_history.hpp"

#include "handlers/common.hpp"
#include "render/personal_account/recipe/moderation_history.hpp"
#include "render/personal_account/recipe/view.hpp"

namespace cookcookhnya::handlers::personal_account::recipe {

using namespace render::personal_account::recipe;

void handleCustomRecipePublicationHistoryCQ(
    CustomRecipePublicationHistory& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::ApiClientRef api) {
    const std::string data = cq.data;
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    if (data == "back") {
        auto ingredientsAndRecipeName = renderCustomRecipe(true, userId, chatId, state.recipeId, bot, api);
        stateManager.put(CustomRecipeView{.recipeId = state.recipeId,
                                          .pageNo = state.pageNo,
                                          .ingredients = ingredientsAndRecipeName.first,
                                          .recipeName = ingredientsAndRecipeName.second});
        return;
    }

    if (data == "confirm") {
        // Peeking (if button with this data then accepted or pending)
        api.getRecipesApi().publishCustom(userId, state.recipeId);
        const bool isPeeking = true;
        renderPublicationHistory(userId, chatId, state.recipeId, state.recipeName, isPeeking, bot, api);
        stateManager.put(states::CustomRecipePublicationHistory{
            .recipeId = state.recipeId, .pageNo = state.pageNo, .recipeName = state.recipeName});
        bot.answerCallbackQuery(cq.id);
        return;
    }
}

} // namespace cookcookhnya::handlers::personal_account::recipe
