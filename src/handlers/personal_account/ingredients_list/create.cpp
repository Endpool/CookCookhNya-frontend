#include "create.hpp"

#include "backend/models/ingredient.hpp"
#include "handlers/common.hpp"
#include "message_tracker.hpp"
#include "render/personal_account/ingredients_list/create.hpp"
#include "render/personal_account/ingredients_list/view.hpp"
#include "states.hpp"
#include "utils/utils.hpp"

namespace cookcookhnya::handlers::personal_account::ingredients {

using namespace render::personal_account::ingredients;

void handleCustomIngredientCreationEnterNameMsg(
    CustomIngredientCreationEnterName& state, MessageRef m, BotRef& bot, SMRef stateManager, IngredientsApiRef api) {
    auto name = m.text;
    auto userId = m.from->id;
    auto chatId = m.chat->id;

    auto text = utils::utf8str(u8"🥦 Введите новое имя ингредиента");
    auto messageId = message::getMessageId(userId);
    if (messageId) {
        bot.editMessageText(text, chatId, *messageId);
    }
    renderCustomIngredientConfirmation(name, userId, chatId, bot, api);
    stateManager.put(CustomIngredientConfirmation{.pageNo = state.pageNo, .name = name});
}

void handleCustomIngredientCreationEnterNameCQ(CustomIngredientCreationEnterName& state,
                                               CallbackQueryRef cq,
                                               BotRef& bot,
                                               SMRef stateManager,
                                               IngredientsApiRef api) {
    bot.answerCallbackQuery(cq.id);
    auto userId = cq.from->id;
    auto chatId = cq.message->chat->id;
    if (cq.data == "back") {
        renderCustomIngredientsList(true, 0, userId, chatId, bot, api);
        stateManager.put(CustomIngredientsList{.pageNo = state.pageNo});
    }
}

void handleCustomIngredientConfirmationCQ(
    CustomIngredientConfirmation& state, CallbackQueryRef cq, BotRef& bot, SMRef stateManager, IngredientsApiRef api) {
    bot.answerCallbackQuery(cq.id);
    auto userId = cq.from->id;
    auto chatId = cq.message->chat->id;
    auto name = state.name;
    if (cq.data == "confirm") {
        api.createCustom(userId, api::models::ingredient::IngredientCreateBody{name});
        renderCustomIngredientsList(true, 0, userId, chatId, bot, api);
        stateManager.put(CustomIngredientsList{.pageNo = state.pageNo});
    }
    if (cq.data == "back") {
        renderCustomIngredientsList(true, 0, userId, chatId, bot, api);
        stateManager.put(CustomIngredientsList{.pageNo = state.pageNo});
    }
}

} // namespace cookcookhnya::handlers::personal_account::ingredients
