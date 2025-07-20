#include "view.hpp"

#include "handlers/common.hpp"
#include "render/personal_account/recipe/publication_history.hpp"
#include "render/personal_account/recipe/search_ingredients.hpp"
#include "render/personal_account/recipes_list/view.hpp"
#include "states.hpp"

#include <cstddef>
#include <ranges>
#include <string>
#include <utility>

namespace cookcookhnya::handlers::personal_account::recipes {

using namespace render::personal_account::recipes;
using namespace render::recipe::ingredients;
using namespace render::personal_account::recipe::publication_history;

using namespace std::views;

const std::size_t numOfIngredientsOnPage = 5;

void handleRecipeCustomViewCQ(
    RecipeCustomView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
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
        // Not peeking (if button with this data then idle or rejected)
        bool isPeeking = false;
        renderPublicationHistory(userId, chatId, state.recipeId, state.recipeName, isPeeking, bot, api);
        stateManager.put(states::CustomRecipePublicationHistory{
            .recipeId = state.recipeId, .pageNo = state.pageNo, .recipeName = state.recipeName});
        bot.answerCallbackQuery(cq.id);
        return;
    }
    if (data == "peekpublish") {
        // Peeking (if button with this data then accepted or pending)
        bool isPeeking = true;
        renderPublicationHistory(userId, chatId, state.recipeId, state.recipeName, isPeeking, bot, api);
        stateManager.put(states::CustomRecipePublicationHistory{
            .recipeId = state.recipeId, .pageNo = state.pageNo, .recipeName = state.recipeName});
        bot.answerCallbackQuery(cq.id);
        return;
    }
}

} // namespace cookcookhnya::handlers::personal_account::recipes
