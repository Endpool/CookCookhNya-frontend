#include "view.hpp"

#include "backend/api/api.hpp"
#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/personal_account/recipe/view.hpp"
#include "render/personal_account/recipes_list/create.hpp"
#include "render/personal_account/recipes_list/view.hpp"
#include "render/personal_account/view.hpp"
#include "states.hpp"
#include "utils/parsing.hpp"

#include <string>
#include <string_view>

namespace cookcookhnya::handlers::personal_account::recipes_list {

using namespace render::personal_account;
using namespace render::personal_account::recipe;
using namespace render::personal_account::recipes_list;
using namespace std::literals;

void handleCustomRecipesListCQ(
    CustomRecipesList& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::ApiClientRef api) {
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
        stateManager.put(CreateCustomRecipe{.pageNo = state.pageNo});
        bot.answerCallbackQuery(cq.id);
        return;
    }

    if (data.starts_with("recipe_")) {
        auto recipeId = utils::parseSafe<api::RecipeId>(std::string_view{data}.substr("recipe_"sv.size()));
        if (recipeId) {
            auto ingredientsAndName = renderCustomRecipe(true, userId, chatId, recipeId.value(), bot, api);
            stateManager.put(CustomRecipeView{.recipeId = recipeId.value(),
                                              .pageNo = state.pageNo,
                                              .ingredients = ingredientsAndName.first,
                                              .recipeName = ingredientsAndName.second});
        }
        bot.answerCallbackQuery(cq.id);
        return;
    }

    if (data != "dont_handle") {
        if (data == "page_left")
            state.pageNo--;
        else if (data == "page_right")
            state.pageNo++;
        renderCustomRecipesList(state.pageNo, userId, chatId, bot, api);
        return;
    }
}

} // namespace cookcookhnya::handlers::personal_account::recipes_list
