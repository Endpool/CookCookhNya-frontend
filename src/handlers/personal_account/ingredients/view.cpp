#include "view.hpp"

#include "handlers/common.hpp"
#include "render/personal_account/ingredients/create.hpp"
#include "render/personal_account/ingredients/publish.hpp"
#include "render/personal_account/view.hpp"

#include "states.hpp"

namespace cookcookhnya::handlers::custom_ingredients_view {

using namespace render::create_custom_ingredient;
using namespace render::publish_custom_ingredients;
using namespace render::personal_account;

void customIngredientsList(
    CustomIngredientsList& /*unused*/, CallbackQueryRef cq, BotRef& bot, SMRef stateManager, ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    auto userId = cq.from->id;
    auto chatId = cq.message->chat->id;
    if (cq.data == "create") {
        renderCustomIngredientCreate(userId, chatId, bot);
        stateManager.put(CustomIngredientCreationEnterName{});
        return;
    }
    if (cq.data == "publish") {
        renderCustomIngredientPublish(userId, chatId, bot, api);
        stateManager.put(CustomIngredientPublish{});
        return;
    }
    if (cq.data == "back") {
        renderPersonalAccountMenu(userId, chatId, bot);
        stateManager.put(PersonalAccountMenu{});
        return;
    }
}
} // namespace cookcookhnya::handlers::custom_ingredients_view
