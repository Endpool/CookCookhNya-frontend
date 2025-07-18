#include "view.hpp"

#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/main_menu/view.hpp"
#include "render/recipe/view.hpp"
#include "render/recipes_suggestions/view.hpp"
#include "render/storage/view.hpp"
#include "render/storages_selection/view.hpp"
#include "states.hpp"
#include "utils/parsing.hpp"

#include <string>
#include <utility>

namespace cookcookhnya::handlers::recipes_suggestions {

using namespace render::recipes_suggestions;
using namespace render::select_storages;
using namespace render::storage;
using namespace render::recipe;
using namespace render::main_menu;

void handleSuggestedRecipeListCQ(
    SuggestedRecipeList& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    auto data = cq.data;

    if (data == "back") {
        if (state.fromStorage) {
            renderStorageView(state.storageIds[0], cq.from->id, chatId, bot, api);
            stateManager.put(StorageView{state.storageIds[0]}); // Go to the only storage
        } else {
            if (api.getStoragesApi().getStoragesList(userId).size() == 1) {
                renderMainMenu(true, userId, chatId, bot, api);
                stateManager.put(MainMenu{});
            } else {
                auto newState = StoragesSelection{.storageIds = std::move(state.storageIds)};
                renderStorageSelection(newState, userId, chatId, bot, api);
                stateManager.put(std::move(newState));
            }
        }
        bot.answerCallbackQuery(cq.id);
        return;
    }

    if (data[0] == 'r') { // Same naive implementation: if first char is r then it's recipe
        auto recipeId = utils::parseSafe<api::RecipeId>(
            data.substr(data.find(' ', 0) + 1, data.size())); // +1 is to move from space and get pure number
        if (recipeId) {
            renderRecipeView(state.storageIds, *recipeId, userId, chatId, bot, api);
            stateManager.put(RecipeView{.storageIds = state.storageIds,
                                        .recipeId = *recipeId,
                                        .fromStorage = state.fromStorage,
                                        .pageNo = state.pageNo});
        }

        return;
    }

    if (data != "dont_handle") {
        auto pageNo = utils::parseSafe<int>(data);
        if (pageNo) {
            state.pageNo = *pageNo;
        }
        // Message is 100% exists as it was rendered by some another method
        renderRecipesSuggestion(state.storageIds, *pageNo, userId, chatId, bot, api);
        return;
    }
}

} // namespace cookcookhnya::handlers::recipes_suggestions
