#include "view.hpp"

#include "backend/api/api.hpp"
#include "handlers/common.hpp"
#include "render/personal_account/recipe/moderation_history.hpp"
#include "render/personal_account/recipe/search_ingredients.hpp"
#include "render/personal_account/recipes_list/view.hpp"
#include "states.hpp"

#include <cstddef>
#include <ranges>
#include <string>
#include <utility>

namespace cookcookhnya::handlers::personal_account::recipe {

using namespace render::personal_account::recipes_list;
using namespace render::personal_account::recipe;
using namespace std::views;

const std::size_t numOfIngredientsOnPage = 5;

void handleRecipeCustomViewCQ(
    CustomRecipeView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::ApiClientRef api) {
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
        bot.answerCallbackQuery(cq.id);
        return;
    }

    if (data == "publish") {
        auto history = api.getRecipesApi().getRecipeRequestHistory(userId, state.recipeId);
        std::string temp;
        renderPublicationHistory(userId, chatId, state.recipeName, temp, history, bot);
        stateManager.put(CustomRecipePublicationHistory{
            .recipeId = state.recipeId, .pageNo = state.pageNo, .recipeName = state.recipeName, .errorReport = ""});
        bot.answerCallbackQuery(cq.id);
        return;
    }
}

} // namespace cookcookhnya::handlers::personal_account::recipe
