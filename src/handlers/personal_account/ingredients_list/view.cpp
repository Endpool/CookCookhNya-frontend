#include "view.hpp"

#include "handlers/common.hpp"
#include "render/personal_account/ingredients_list/create.hpp"
#include "render/personal_account/ingredients_list/publish.hpp"
#include "render/personal_account/ingredients_list/view.hpp"
#include "render/personal_account/view.hpp"

#include "states.hpp"
#include "utils/parsing.hpp"
#include <cstddef>

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
    auto pageNo = utils::parseSafe<std::size_t>(cq.data);
    if (pageNo) {
        renderCustomIngredientsList(true, *pageNo, userId, chatId, bot, api);
        stateManager.put(CustomIngredientsList{.pageNo = *pageNo});
        return;
    }
}
} // namespace cookcookhnya::handlers::personal_account::ingredients
