#include "recipes_suggestion_list.hpp"
#include "render/recipes_suggestion/recipes_suggestion_render.hpp"
#include "states.hpp"

namespace cookcookhnya::handlers::recipies_suggestion {
using namespace render::recipes_suggestion;

void changePageAndBack(SuggestedRecipeList& state,
                       CallbackQueryRef cq,
                       BotRef bot,
                       SMRef stateManager,
                       StorageApiRef storageApi) { // MUST BE RECIPES API
    auto chatId = cq.message->chat->id;
    auto messageId = cq.message->messageId;
    std::string delimiter = " ";
    auto data = cq.data;
    std::stringstream temp; // To convert string to int

    // About pageNo - got initialised one already from state from which it goes as 1!
    if (data[0] == 'b') { // Here is quite naive implementation: if first char is b then it's "backFromSuggestedRecipes"
        temp << data.substr(0, data.find(delimiter) + 1); // +1 is to move from space and get pure number
        int numOfStorages = 0;
        temp >> numOfStorages;
        if (numOfStorages > 1) {
            stateManager.put(
                StorageSelection{state.storageIds}); // Go to storages selection saving the storages which were chosen
            //  RENDER FROM AMIRKHAN render_storages_select()
        } else {
            stateManager.put(StorageView{state.storageIds[0]}); // Go to the storage (idk what's wrong with linter)
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

    editSuggestionMessage(state.storageIds,
                          pageNo,
                          chatId,
                          messageId,
                          bot,
                          storageApi); // Message is 100% exists as it was rendered by some another method
}
} // namespace cookcookhnya::handlers::recipies_suggestion
