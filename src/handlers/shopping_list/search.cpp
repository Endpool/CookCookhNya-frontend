#include "search.hpp"

#include "backend/api/api.hpp"
#include "backend/api/publicity_filter.hpp"
#include "backend/id_types.hpp"
#include "backend/models/shopping_list.hpp"
#include "handlers/common.hpp"
#include "render/shopping_list/search.hpp"
#include "render/shopping_list/view.hpp"
#include "utils/parsing.hpp"

#include <cstddef>
#include <string_view>
#include <utility>

namespace cookcookhnya::handlers::shopping_list {

using namespace render::shopping_list;
using namespace api;
using namespace api::models::shopping_list;

const std::size_t searchThreshold = 70;

void handleShoppingListIngredientSearchCQ(
    ShoppingListIngredientSearch& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    auto userId = cq.from->id;
    auto chatId = cq.message->chat->id;

    if (cq.data == "back") {
        renderShoppingList(state.prevState, userId, chatId, bot);
        stateManager.put(auto{std::move(state.prevState)});
        return;
    }

    if (cq.data == "page_left" || cq.data == "page_right") {
        if (cq.data == "page_left") {
            if (state.pagination.pageNo == 0)
                return;
            state.pagination.pageNo--;
        } else {
            if (state.pagination.totalItems <= state.pagination.pageNo * searchPageSize)
                return;
            state.pagination.pageNo++;
        }
        auto result = api.getIngredientsApi().search(userId,
                                                     PublicityFilterType::All,
                                                     state.query,
                                                     searchPageSize,
                                                     searchPageSize * state.pagination.pageNo,
                                                     searchThreshold);
        state.page = result.page;
        renderShoppingListIngredientSearch(state, searchPageSize, userId, chatId, bot);
        return;
    }

    if (cq.data.starts_with("ingredient_")) {
        auto mIngredient = utils::parseSafe<IngredientId>(std::string_view{cq.data}.substr(sizeof("ingredient_") - 1));
        if (!mIngredient)
            return;
        api.getShoppingListApi().put(userId, {*mIngredient});
        auto ingredientInfo = api.getIngredientsApi().get(userId, *mIngredient);
        state.prevState.items.put({ShoppingListItem{.ingredientId = ingredientInfo.id, .name = ingredientInfo.name}});
        return;
    }
}

void handleShoppingListIngredientSearchIQ(ShoppingListIngredientSearch& state,
                                          InlineQueryRef iq,
                                          BotRef bot,
                                          api::IngredientsApiRef api) {
    auto userId = iq.from->id;
    if (iq.query.empty()) {
        state.query = "";
        state.pagination = {};
        state.page = {};
        renderShoppingListIngredientSearch(state, searchPageSize, userId, userId, bot);
        return;
    }

    auto result = api.search(userId, PublicityFilterType::All, iq.query, searchPageSize, 0, searchThreshold);
    state.query = iq.query;
    state.pagination.pageNo = 0;
    state.pagination.totalItems = result.found;
    state.page = result.page;
    renderShoppingListIngredientSearch(state, searchPageSize, userId, userId, bot);
}

} // namespace cookcookhnya::handlers::shopping_list
