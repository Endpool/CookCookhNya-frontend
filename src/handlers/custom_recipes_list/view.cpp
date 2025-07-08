#include "view.hpp"

#include "handlers/common.hpp"
#include "render/custom_recipes_list/create.hpp"
#include "render/custom_recipes_list/view.hpp"
#include "render/personal_account/view.hpp"
#include "states.hpp"
#include "utils.hpp"
#include <string>

namespace cookcookhnya::handlers::custom_recipes_list {

void customRecipeList(CustomRecipesList& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    using namespace render::personal_account;
    using namespace render::custom_recipes_list;
    using namespace render::create_custom_recipe;

    bot.answerCallbackQuery(cq.id);

    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;
    auto data = cq.data;

    if (data[0] == 'b') {
        renderPersonalAccountMenu(userId, chatId, bot);
        stateManager.put(PersonalAccountMenu{});
        bot.answerCallbackQuery(cq.id);
        return;
    }
    if (data == "custom_recipe_create") {
        renderRecipeCreate(chatId, userId, bot);
        stateManager.put(CreateCustomRecipe{
            .recipeId =
                0}); // Some default value which will be assigned later from backend as we will know name of recipe
    }

    /*if (data[0] == 'r') { // Same naive implementation: if first char is r then it's recipe

        auto recipeId = utils::parseSafe<api::RecipeId>(
            data.substr(data.find(' ', 0) + 1, data.size())); // +1 is to move from space and get pure number
        if (recipeId) {
            renderRecipeViewAfterAddingStorage(state.storageIds, *recipeId, userId, chatId, bot, api);
            stateManager.put(RecipeView{.storageIds = state.storageIds,
                                        .recipeId = *recipeId,
                                        .fromStorage = state.fromStorage,
                                        .pageNo = state.pageNo});
        }

        return;
    }*/

    if (data != "ㅤ") { // If it's not "empty" button then it's change of page
        auto pageNo = utils::parseSafe<int>(data);
        if (pageNo) {
            state.pageNo = *pageNo;
        }

        renderCustomRecipesList(*pageNo, userId, chatId, bot, api);
        return;
    }
}

} // namespace cookcookhnya::handlers::custom_recipes_list
