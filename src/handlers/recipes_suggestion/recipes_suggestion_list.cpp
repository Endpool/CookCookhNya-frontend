#include "recipes_suggestion_list.hpp"

#include "handlers/common.hpp"
#include "render/recipes_suggestion/recipes_suggestion_render.hpp"
#include "render/recipes_suggestion/select_storages_render.hpp"
#include "states.hpp"

#include <sstream>
#include <string>
#include <utility>

namespace cookcookhnya::handlers::recipes_suggestion {

using namespace render::recipes_suggestion;
using namespace render::select_storages;

void changePageAndBack(
    SuggestedRecipeList& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {

    auto chatId = cq.message->chat->id;
    auto messageId = cq.message->messageId;
    auto userId = cq.from->id;

    std::string delimiter = " ";
    auto data = cq.data;
    std::stringstream temp; // To convert string to int

    // About pageNo - got initialised one already from state from which it goes as 1!
    if (data[0] == 'b') { // Here is quite naive implementation: if first char is b then it's "backFromSuggestedRecipes"
        temp << data.substr(0, data.find(delimiter) + 1); // +1 is to move from space and get pure number
        int numOfStorages = 0;
        temp >> numOfStorages;
        auto strgs = state.storageIds;
        if (numOfStorages > 1) {
            // Go to storages selection saving the storages which were chosen
            auto message = renderStoragesSelect(strgs, userId, chatId, bot, api);
            stateManager.put(StorageSelection{.storageIds = std::move(state.storageIds), .messageId = message});
        } else {
            stateManager.put(StorageView{strgs[0]}); // Go to the only one storage (idk what's wrong with
                                                     // linter), index is 0 as the object is only one
        }
        return;
    }

    if (data[0] == 'r') { // Same naive implementation: if first char is r then it's recipe
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
