#include "view.hpp"

#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/personal_account/recipe/view.hpp"
#include "render/personal_account/recipes_list/create.hpp"
#include "render/personal_account/recipes_list/view.hpp"
#include "render/personal_account/view.hpp"
#include "states.hpp"
#include "utils/parsing.hpp"

#include <string>

namespace cookcookhnya::handlers::personal_account::recipes {

void handleCustomRecipesListCQ(
    CustomRecipesList& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    using namespace render::personal_account;
    using namespace render::personal_account::recipes;

    bot.answerCallbackQuery(cq.id);

    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;
    auto data = cq.data;

    if (data == "back") {
        renderPersonalAccountMenu(userId, chatId, bot);
        stateManager.put(PersonalAccountMenu{});
        bot.answerCallbackQuery(cq.id);
        return;
    }
    if (data == "custom_recipe_create") {
        renderRecipeCreation(chatId, userId, bot);
        stateManager.put(CreateCustomRecipe{.recipeId = {}, .pageNo = state.pageNo});
        bot.answerCallbackQuery(cq.id);
        return;
    }

    if (data[0] == 'r') {
        auto recipeId = utils::parseSafe<api::RecipeId>(data.substr(1, data.size()));
        if (recipeId) {
            auto ingredients = renderCustomRecipe(true, userId, chatId, recipeId.value(), bot, api);
            stateManager.put(
                RecipeCustomView{.recipeId = recipeId.value(), .pageNo = state.pageNo, .ingredients = ingredients});
        }
        return;
    }

    if (data != "dont_handle") {
        auto pageNo = utils::parseSafe<int>(data);
        if (pageNo) {
            state.pageNo = *pageNo;
        }
        renderCustomRecipesList(*pageNo, userId, chatId, bot, api);
        return;
    }
}

} // namespace cookcookhnya::handlers::personal_account::recipes
