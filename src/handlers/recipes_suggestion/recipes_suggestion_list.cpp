#include "recipes_suggestion_list.hpp"

#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/recipes_suggestion/recipe_view_render.hpp"
#include "render/recipes_suggestion/recipes_suggestion_render.hpp"
#include "render/recipes_suggestion/select_storages_render.hpp"
#include "render/storage_view/storage_view_render.hpp"
#include "states.hpp"

#include <sstream>
#include <string>
#include <utility>

namespace cookcookhnya::handlers::recipes_suggestion {

using namespace render::recipes_suggestion;
using namespace render::select_storages;
using namespace render::storage;
using namespace render::recipe_view;


void changePageAndBack(
    SuggestedRecipeList& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);

    auto chatId = cq.message->chat->id;
    auto messageId = cq.message->messageId;
    auto userId = cq.from->id;

    auto data = cq.data;
    std::stringstream temp; // To convert string to int

    if (data[0] == 'b') { // Here is quite naive implementation: if first char is b then it's "backFromSuggestedRecipes"
        if (state.fromStorage) {
            renderStorageView(state.storageIds[0], cq.from->id, chatId, bot, api);
            stateManager.put(StorageView{state.storageIds[0]}); // Go to the only one storage
        } else {
            // Go to storages selection saving the storages which were chosen
            auto message = renderStoragesSelect(state.storageIds, userId, chatId, bot, api);
            stateManager.put(StorageSelection{.storageIds = std::move(state.storageIds), .messageId = message});
        }
        bot.answerCallbackQuery(cq.id);
        return;
    }

    if (data[0] == 'r') { // Same naive implementation: if first char is r then it's recipe

        temp << data.substr(data.find("_", 0) + 1, data.size()); // +1 is to move from space and get pure number
        api::RecipeId recipeId = 0;
        temp >> recipeId;

        renderRecipeView(state.storageIds, recipeId, userId, chatId, bot, api);
        stateManager.put(RecipeView{.storageIds = state.storageIds, .recipeId = recipeId});
        // TODO: Add render of moving to recipe and render this state
        //  data already had recipeId in it
        return;
    }

    // If none of if's worked then the first char is number -> it's about next or prev page

    int pageNo = 0;
    temp << data;
    temp >> pageNo;

    // Message is 100% exists as it was rendered by some another method
    editSuggestionMessage(state.storageIds, pageNo, userId, chatId, messageId, bot, api);
}
} // namespace cookcookhnya::handlers::recipes_suggestion
