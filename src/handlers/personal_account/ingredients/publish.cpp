#include "publish.hpp"

#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/personal_account/ingredients/view.hpp"

#include "states.hpp"
#include "utils/utils.hpp"

namespace cookcookhnya::handlers::publish_custom_ingredients {

using namespace render::view_custom_ingredients;

void customIngredientPublish(
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
} // namespace cookcookhnya::handlers::publish_custom_ingredients
