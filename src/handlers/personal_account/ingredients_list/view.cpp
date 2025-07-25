#include "view.hpp"

#include "backend/api/api.hpp"
#include "handlers/common.hpp"
#include "render/personal_account/ingredients_list/create.hpp"
#include "render/personal_account/ingredients_list/delete.hpp"
#include "render/personal_account/ingredients_list/publish.hpp"
#include "render/personal_account/view.hpp"
#include "states.hpp"

namespace cookcookhnya::handlers::personal_account::ingredients {

using namespace render::personal_account::ingredients;
using namespace render::personal_account;

void handleCustomIngredientsListCQ(
    CustomIngredientsList& /*unused*/, CallbackQueryRef cq, BotRef& bot, SMRef stateManager, api::ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    auto userId = cq.from->id;
    auto chatId = cq.message->chat->id;
    if (cq.data == "create") {
        renderCustomIngredientCreation(userId, chatId, bot);
        stateManager.put(CustomIngredientCreationEnterName{});
        return;
    }
    if (cq.data == "delete") {
        renderCustomIngredientDeletion(userId, chatId, bot, api);
        stateManager.put(CustomIngredientDeletion{});
        return;
    }
    if (cq.data == "publish") {
        renderCustomIngredientPublication(userId, chatId, bot, api);
        stateManager.put(CustomIngredientPublish{});
        return;
    }
    if (cq.data == "back") {
        renderPersonalAccountMenu(userId, chatId, bot);
        stateManager.put(PersonalAccountMenu{});
        return;
    }
}
} // namespace cookcookhnya::handlers::personal_account::ingredients
