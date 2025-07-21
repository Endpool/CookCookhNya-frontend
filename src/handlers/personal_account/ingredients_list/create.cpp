#include "create.hpp"

#include "backend/api/ingredients.hpp"
#include "backend/models/ingredient.hpp"
#include "handlers/common.hpp"
#include "message_tracker.hpp"
#include "render/personal_account/ingredients_list/create.hpp"
#include "render/personal_account/ingredients_list/view.hpp"
#include "render/personal_account/recipe/search_ingredients.hpp"
#include "render/storage/ingredients/view.hpp"
#include "utils/utils.hpp"

#include <ranges>

namespace cookcookhnya::handlers::personal_account::ingredients {

using namespace render::storage::ingredients;
using namespace render::personal_account::recipe;
using namespace render::personal_account::ingredients;
using namespace std::views;

void handleCustomIngredientCreationEnterNameMsg(CustomIngredientCreationEnterName& /*unused*/,
                                                MessageRef m,
                                                BotRef& bot,
                                                SMRef stateManager,
                                                api::IngredientsApiRef api) {
    auto name = m.text;
    auto userId = m.from->id;
    auto chatId = m.chat->id;

    auto text = utils::utf8str(u8"🥦 Введите новое имя ингредиента");
    auto messageId = message::getMessageId(userId);
    if (messageId) {
        bot.editMessageText(text, chatId, *messageId);
    }
    renderCustomIngredientConfirmation(false, name, userId, chatId, bot, api);
    stateManager.put(CustomIngredientConfirmation{name});
}

void handleCustomIngredientCreationEnterNameCQ(CustomIngredientCreationEnterName& state,
                                               CallbackQueryRef cq,
                                               BotRef& bot,
                                               SMRef stateManager,
                                               api::IngredientsApiRef api) {
    bot.answerCallbackQuery(cq.id);
    auto userId = cq.from->id;
    auto chatId = cq.message->chat->id;
    if (cq.data == "back") {
        renderCustomIngredientsList(true, 0, userId, chatId, bot, api);
        stateManager.put(CustomIngredientsList{.pageNo = state.pageNo});
    }
}

void handleCustomIngredientConfirmationCQ(
    CustomIngredientConfirmation& state, CallbackQueryRef cq, BotRef& bot, SMRef stateManager, api::ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    auto userId = cq.from->id;
    auto chatId = cq.message->chat->id;
    auto name = state.name;

    if (cq.data == "confirm") {
        api.getIngredientsApi().createCustom(userId, api::models::ingredient::IngredientCreateBody{name});
        renderCustomIngredientsList(true, state.pageNo, userId, chatId, bot, api);
        stateManager.put(CustomIngredientsList{});
    }

    if (cq.data == "back") {
        const std::size_t numOfIngredientsOnPage = 5;

        if (state.recipeFrom.has_value()) {
            auto newState =
                CustomRecipeIngredientsSearch{state.recipeFrom.value(), state.ingredients.value() | as_rvalue, ""};
            renderRecipeIngredientsSearch(newState, numOfIngredientsOnPage, userId, chatId, bot);
            stateManager.put(std::move(newState));
            return;
        }

        if (state.storageFrom.has_value()) {
            auto ingredients = api.getIngredientsApi().getStorageIngredients(userId, state.storageFrom.value());
            auto newState = StorageIngredientsList{state.storageFrom.value(), ingredients | as_rvalue, ""};
            renderIngredientsListSearch(newState, userId, chatId, bot);
            stateManager.put(std::move(newState));
            return;
        }

        renderCustomIngredientsList(true, state.pageNo, userId, chatId, bot, api);
        stateManager.put(CustomIngredientsList{});
        return;
    }
}

} // namespace cookcookhnya::handlers::personal_account::ingredients
