#include "publish.hpp"

#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/personal_account/ingredients_list/view.hpp"
#include "states.hpp"
#include "utils/parsing.hpp"

namespace cookcookhnya::handlers::personal_account::ingredients {

using namespace render::personal_account::ingredients;

void handleCustomIngredientPublishCQ(
    CustomIngredientPublish& /*unused*/, CallbackQueryRef cq, BotRef& bot, SMRef stateManager, IngredientsApiRef api) {
    bot.answerCallbackQuery(cq.id);
    auto userId = cq.from->id;
    auto chatId = cq.message->chat->id;

    auto ingredientId = utils::parseSafe<api::IngredientId>(cq.data);
    if (ingredientId) {
        api.publishCustom(userId, *ingredientId);
    }
    renderCustomIngredientsList(true, userId, chatId, bot, api);
    stateManager.put(CustomIngredientsList{});
}
} // namespace cookcookhnya::handlers::personal_account::ingredients
