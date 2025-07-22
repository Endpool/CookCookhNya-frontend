#include "view.hpp"

#include "handlers/common.hpp"
#include "render/recipes_search/view.hpp"

#include <utility>

namespace cookcookhnya::handlers::recipe {

using namespace render::recipes_search;

void handleRecipeViewCQ(RecipeView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager) {
    bot.answerCallbackQuery(cq.id);
    auto userId = cq.from->id;
    auto chatId = cq.message->chat->id;

    if (cq.data == "back") {
        renderRecipesSearch(state.prevState.pagination, state.prevState.page, userId, chatId, bot);
        stateManager.put(auto{std::move(state.prevState)});
        return;
    }
}

} // namespace cookcookhnya::handlers::recipe
