#include "view.hpp"

#include "backend/api/api.hpp"
#include "backend/api/publicity_filter.hpp"
#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/main_menu/view.hpp"
#include "render/recipes_search/view.hpp"
#include "utils/parsing.hpp"

#include <algorithm>
#include <cstddef>
#include <string_view>

namespace cookcookhnya::handlers::recipes_search {

using namespace render::main_menu;
using namespace render::recipes_search;
// using namespace render::recipe;
using namespace std::literals;

void handleRecipesSearchCQ(
    RecipesSearch& /*unused*/, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    auto userId = cq.from->id;
    auto chatId = cq.message->chat->id;

    if (cq.data == "back") {
        renderMainMenu(true, std::nullopt, userId, chatId, bot, api);
        stateManager.put(MainMenu{});
        return;
    }

    if (cq.data.starts_with("recipe_")) {
        auto recipeId = utils::parseSafe<api::RecipeId>(std::string_view{cq.data}.substr("recipe_"sv.size()));
        renderMainMenu(true, std::nullopt, userId, chatId, bot, api);
        stateManager.put(MainMenu{});
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
        const std::size_t pageSize = 5;
        auto result = api.search(userId, PublicityFilterType::All, state.query, pageSize);
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
