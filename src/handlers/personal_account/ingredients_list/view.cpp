#include "view.hpp"

#include "handlers/common.hpp"
#include "render/personal_account/ingredients_list/create.hpp"
#include "render/personal_account/ingredients_list/publish.hpp"
#include "render/personal_account/ingredients_list/view.hpp"
#include "render/personal_account/view.hpp"

#include "states.hpp"

namespace cookcookhnya::handlers::personal_account::ingredients {

using namespace render::personal_account::ingredients;
using namespace render::personal_account;

void handleCustomIngredientsListCQ(
    CustomIngredientsList& state, CallbackQueryRef cq, BotRef& bot, SMRef stateManager, ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    auto userId = cq.from->id;
    auto chatId = cq.message->chat->id;
    if (cq.data == "create") {
        renderCustomIngredientCreation(userId, chatId, bot);
        stateManager.put(CustomIngredientCreationEnterName{.pageNo = state.pageNo});
        return;
    }
    if (cq.data == "publish") {
        renderCustomIngredientPublication(userId, chatId, bot, api);
        stateManager.put(CustomIngredientPublish{.pageNo = state.pageNo});
        return;
    }
    if (cq.data == "back") {
        renderPersonalAccountMenu(userId, chatId, bot);
        stateManager.put(PersonalAccountMenu{});
        return;
    }
    if (cq.data == "prev") {
        state.pageNo -= 1;
        renderCustomIngredientsList(true, state.pageNo, userId, chatId, bot, api);
        stateManager.put(CustomIngredientsList{.pageNo = state.pageNo});
        return;
    }
    if (cq.data == "next") {
        state.pageNo += 1;
        renderCustomIngredientsList(true, state.pageNo, userId, chatId, bot, api);
        stateManager.put(CustomIngredientsList{.pageNo = state.pageNo});
        return;
    }
}
} // namespace cookcookhnya::handlers::personal_account::ingredients
