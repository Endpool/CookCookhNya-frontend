#include "create.hpp"

#include "backend/models/ingredient.hpp"
#include "handlers/common.hpp"
#include "message_tracker.hpp"
#include "render/personal_account/ingredients/create.hpp"
#include "render/personal_account/ingredients/view.hpp"
#include "states.hpp"
#include "utils.hpp"

namespace cookcookhnya::handlers::create_custom_ingredients {

using namespace render::view_custom_ingredients;
using namespace render::create_custom_ingredient;

void customIngredientEnterName(CustomIngredientCreationEnterName& /*unused*/,
                               MessageRef m,
                               BotRef& bot,
                               SMRef stateManager,
                               IngredientsApiRef api) {
    auto name = m.text;
    auto userId = m.from->id;
    auto chatId = m.chat->id;

    auto text = utils::utf8str(u8"🥦 Введите новое имя ингредиента");
    auto messageId = message::getMessageId(userId);
    if (messageId) {
        bot.editMessageText(text, chatId, *messageId);
    }
    renderCustomIngredientConfirm(name, userId, chatId, bot, api);
    stateManager.put(CustomIngredientConfirmation{name});
}

void customIngredientCancelCreation(CustomIngredientCreationEnterName& /*unused*/,
                                    CallbackQueryRef cq,
                                    BotRef& bot,
                                    SMRef stateManager,
                                    IngredientsApiRef api) {
    bot.answerCallbackQuery(cq.id);
    auto userId = cq.from->id;
    auto chatId = cq.message->chat->id;
    if (cq.data == "cancel") {
        renderCustomIngredientsList(true, userId, chatId, bot, api);
        stateManager.put(CustomIngredientsList{});
    }
}

void customIngredientConfirmation(
    CustomIngredientConfirmation& state, CallbackQueryRef cq, BotRef& bot, SMRef stateManager, IngredientsApiRef api) {
    bot.answerCallbackQuery(cq.id);
    auto userId = cq.from->id;
    auto chatId = cq.message->chat->id;
    auto name = state.name;
    if (cq.data == "confirm") {
        api.createCustom(userId, api::models::ingredient::IngredientCreateBody{name});
        renderCustomIngredientsList(true, userId, chatId, bot, api);
        stateManager.put(CustomIngredientsList{});
    }
    if (cq.data == "cancel") {
        renderCustomIngredientsList(true, userId, chatId, bot, api);
        stateManager.put(CustomIngredientsList{});
    }
}

} // namespace cookcookhnya::handlers::create_custom_ingredients
