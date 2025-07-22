#include "moderation_history.hpp"

#include "backend/models/publication_request_status.hpp"
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

    if (data == "backFromRules") {
        auto history = api.getRecipesApi().getRecipeRequestHistory(userId, state.recipeId);
        renderPublicationHistory(userId, chatId, state.recipeName, state.errorReport, history, bot);
        bot.answerCallbackQuery(cq.id);
        return;
    }
    if (data == "rules") {
        renderPublicationRules(userId, chatId, bot);
        bot.answerCallbackQuery(cq.id);
        return;
    }
    if (data == "back") {
        auto ingredientsAndRecipeName = renderCustomRecipe(true, userId, chatId, state.recipeId, bot, api);
        stateManager.put(CustomRecipeView{.recipeId = state.recipeId,
                                          .pageNo = state.pageNo,
                                          .ingredients = ingredientsAndRecipeName.first,
                                          .recipeName = ingredientsAndRecipeName.second});
        return;
    }

    if (data == "confirm") {

        auto history = api.getRecipesApi().getRecipeRequestHistory(userId, state.recipeId);
        // Here check for emptiness first, thanks to lazy compilator
        const bool shouldPublish = history.empty() || (history.back().status.status ==
                                                       api::models::moderation::PublicationRequestStatus::REJECTED);

        if (shouldPublish) {
            try {
                api.getRecipesApi().publishCustom(userId, state.recipeId);
                state.errorReport = "";
            } catch (...) {
                state.errorReport =
                    utils::utf8str(u8"⚠️Что-то пошло не так, вероятно ваш рецепт содержит неопубликованные ингредиенты");
            }
            // Get updated history
            history = api.getRecipesApi().getRecipeRequestHistory(userId, state.recipeId);
        }
        renderPublicationHistory(userId, chatId, state.recipeName, state.errorReport, history, bot);
        bot.answerCallbackQuery(cq.id);
        return;
    }
}

} // namespace cookcookhnya::handlers::personal_account::recipe
