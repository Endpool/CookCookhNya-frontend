#include "suggest.hpp"

#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/main_menu/view.hpp"
#include "render/recipes_suggestion/recipe/view.hpp"
#include "render/recipes_suggestion/storage_selection/select.hpp"
#include "render/recipes_suggestion/suggest.hpp"
#include "render/storage_view/view.hpp"

#include "states.hpp"
#include "utils.hpp"

#include <string>
#include <utility>

namespace cookcookhnya::handlers::recipes_suggestion {

using namespace render::recipes_suggestion;
using namespace render::select_storages;
using namespace render::storage;
using namespace render::recipe_view;
using namespace render::main_menu;

void changePageAndBack(
    SuggestedRecipeList& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    auto data = cq.data;

    if (data[0] == 'b') { // Here is quite naive implementation: if first char is b then it's "backFromSuggestedRecipes"
        if (state.fromStorage) {
            renderStorageView(state.storageIds[0], cq.from->id, chatId, bot, api);
            stateManager.put(StorageView{state.storageIds[0]}); // Go to the only one storage
        } else {
            if (state.storageIds.size() == 1) {
                renderMainMenu(true, userId, chatId, bot, api);
                stateManager.put(MainMenu{});
            } else {
                // Go to storages selection saving the storages which were chosen
                renderStorageSelect(state.storageIds, userId, chatId, bot, api);
                stateManager.put(StorageSelection{.storageIds = std::move(state.storageIds)});
            }
        }
        bot.answerCallbackQuery(cq.id);
        return;
    }

    if (data[0] == 'r') { // Same naive implementation: if first char is r then it's recipe

        auto recipeId = utils::parseSafe<api::RecipeId>(
            data.substr(data.find(' ', 0) + 1, data.size())); // +1 is to move from space and get pure number
        if (recipeId) {
            renderRecipeViewAfterAddingStorage(state.storageIds, *recipeId, userId, chatId, bot, api);
            stateManager.put(RecipeView{.storageIds = state.storageIds,
                                        .recipeId = *recipeId,
                                        .fromStorage = state.fromStorage,
                                        .pageNo = state.pageNo});
        }

        return;
    }

    if (data != "dontHandle") {
        auto pageNo = utils::parseSafe<int>(data);
        if (pageNo) {
            state.pageNo = *pageNo;
        }
        // Message is 100% exists as it was rendered by some another method
        editRecipesSuggestion(state.storageIds, *pageNo, userId, chatId, bot, api);
        return;
    }
}

} // namespace cookcookhnya::handlers::recipes_suggestion
