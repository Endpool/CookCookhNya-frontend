#include "ingredients_spending.hpp"

#include "backend/api/api.hpp"
#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/cooking/ingredients_spending.hpp"
#include "render/cooking_planning/view.hpp"
#include "states.hpp"
#include "utils/parsing.hpp"
#include "utils/utils.hpp"

#include <algorithm>
#include <ranges>
#include <string_view>
#include <utility>

namespace cookcookhnya::handlers::cooking {

using namespace render::cooking_planning;
using namespace render::cooking;
using states::helpers::SelectableIngredient;
using namespace std::literals;
using namespace std::views;
using std::ranges::to;

void handleCookingIngredientsSpendingCQ(
    CookingIngredientsSpending& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::ApiClientRef api) {
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    if (cq.data == "back") {
        bot.answerCallbackQuery(cq.id);
        renderCookingPlanning(state.prevState.availability, state.prevState.recipeId, userId, chatId, bot, api);
        stateManager.put(auto{std::move(state.prevState)});
        return;
    }

    if (cq.data == "remove") {
        if (!state.storageId)
            return;
        auto selected = state.ingredients | filter(&SelectableIngredient::selected) |
                        transform(&SelectableIngredient::id) | to<std::vector>();
        api.getIngredientsApi().deleteMultipleFromStorage(userId, *state.storageId, selected);
        bot.answerCallbackQuery(cq.id, utils::utf8str(u8"Успешно удалено из выбранного хранилища"));
        return;
    }

    if (cq.data == "to_shopping_list") {
        auto selected = state.ingredients | filter(&SelectableIngredient::selected) |
                        transform(&SelectableIngredient::id) | to<std::vector>();
        api.getShoppingListApi().put(userId, selected);
        bot.answerCallbackQuery(cq.id, utils::utf8str(u8"Успешно добавлено"));
        return;
    }

    if (cq.data.starts_with("ingredient_")) {
        auto mIngredientId =
            utils::parseSafe<api::IngredientId>(std::string_view{cq.data}.substr("ingredient_"sv.size()));
        if (!mIngredientId)
            return;
        auto ingredientIter = std::ranges::find(state.ingredients, *mIngredientId, &SelectableIngredient::id);
        if (ingredientIter == state.ingredients.end())
            return;
        ingredientIter->selected = !ingredientIter->selected;
        renderIngredientsSpending(state.ingredients, state.storageId.has_value(), userId, chatId, bot);
        return;
    }
}

} // namespace cookcookhnya::handlers::cooking
