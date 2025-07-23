#include "view.hpp"

#include "backend/api/api.hpp"
#include "handlers/common.hpp"
#include "render/main_menu/view.hpp"
#include "render/recipes_search/view.hpp"

#include <optional>
#include <utility>

namespace cookcookhnya::handlers::recipe {

using namespace render::recipes_search;
using namespace render::main_menu;

void handleRecipeViewCQ(RecipeView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    auto userId = cq.from->id;
    auto chatId = cq.message->chat->id;

    if (cq.data == "back") {
        if (auto& prevState = state.prevState) {
            renderRecipesSearch(prevState->pagination, prevState->page, userId, chatId, bot);
            stateManager.put(auto{std::move(*prevState)});
        } else {
            renderMainMenu(true, std::nullopt, userId, chatId, bot, api);
            stateManager.put(MainMenu{});
        }
        return;
    }
}

} // namespace cookcookhnya::handlers::recipe
