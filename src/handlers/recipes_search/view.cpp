#include "view.hpp"

#include "backend/api/api.hpp"
#include "backend/api/publicity_filter.hpp"
#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/main_menu/view.hpp"
#include "render/recipe/view.hpp"
#include "render/recipes_search/view.hpp"
#include "utils/parsing.hpp"

#include <algorithm>
#include <cstddef>
#include <string_view>
#include <utility>

namespace cookcookhnya::handlers::recipes_search {

using namespace render::main_menu;
using namespace render::recipes_search;
using namespace render::recipe;
using namespace std::literals;

const std::size_t pageSize = 5;
const unsigned threshold = 70;

void handleRecipesSearchCQ(
    RecipesSearch& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    auto userId = cq.from->id;
    auto chatId = cq.message->chat->id;

    if (cq.data == "back") {
        renderMainMenu(true, std::nullopt, userId, chatId, bot, api);
        stateManager.put(MainMenu{});
        return;
    }

    if (cq.data == "page_left" || cq.data == "page_right") {
        if (cq.data == "page_left") {
            if (state.pagination.pageNo == 0)
                return;
            state.pagination.pageNo--;
        } else {
            if (state.pagination.totalItems <= state.pagination.pageNo * pageSize)
                return;
            state.pagination.pageNo++;
        }
        auto result = api.getRecipesApi().search(
            userId, PublicityFilterType::All, state.query, pageSize, pageSize * state.pagination.pageNo, threshold);
        state.page = result.page;
        renderRecipesSearch(state.pagination, state.page, userId, userId, bot);
        return;
    }

    if (cq.data.starts_with("recipe_")) {
        auto mRecipeId = utils::parseSafe<api::RecipeId>(std::string_view{cq.data}.substr("recipe_"sv.size()));
        if (!mRecipeId)
            return;
        auto recipe = api.getRecipesApi().get(userId, *mRecipeId);
        renderRecipeView(recipe, *mRecipeId, userId, chatId, bot);
        stateManager.put(RecipeView{.prevState = {std::move(state)}, .recipe = std::move(recipe)});
        return;
    }
}

void handleRecipesSearchIQ(RecipesSearch& state, InlineQueryRef iq, BotRef bot, api::RecipesApiRef api) {
    auto userId = iq.from->id;
    if (iq.query.empty()) {
        if (state.query.empty())
            return;
        state.query = "";
        state.pagination = {};
        state.page = {};
    } else {
        auto result = api.search(userId, PublicityFilterType::All, iq.query, pageSize, 0, threshold);
        const auto idGetter = &api::models::recipe::RecipeSummary::id;
        if (std::ranges::equal(result.page, state.page, {}, idGetter, idGetter))
            return;

        state.query = iq.query;
        state.pagination = {.pageNo = 0, .totalItems = result.found};
        state.page = result.page;
    }
    renderRecipesSearch(state.pagination, state.page, userId, userId, bot);
}

} // namespace cookcookhnya::handlers::recipes_search
