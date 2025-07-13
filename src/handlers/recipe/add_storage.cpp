#include "add_storage.hpp"

#include "backend/id_types.hpp"
#include "render/recipe/add_storage.hpp"
#include "render/recipe/view.hpp"
#include "utils.hpp"

namespace cookcookhnya::handlers::recipe {

using namespace render::recipe;

void handleRecipeAddStorage(
    RecipeStorageAddition& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    std::string data = cq.data;
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    if (data[0] == '+') {
        auto newStorageIdStr =
            data.substr(1, data.size()); // Here we got all selected storages and new one as last in string
        auto newStorageId = utils::parseSafe<api::StorageId>(newStorageIdStr);
        if (newStorageId) {
            state.storageIds.push_back(*newStorageId);
            renderStorageSuggestion(state.storageIds, state.recipeId, userId, chatId, bot, api);
        }
        bot.answerCallbackQuery(cq.id);
    }

    if (data == "back_from_adding_storages") {
        renderRecipeViewAfterAddingStorage(state.storageIds, state.recipeId, userId, chatId, bot, api);
        stateManager.put(RecipeView{.storageIds = state.storageIds,
                                    .recipeId = state.recipeId,
                                    .fromStorage = state.fromStorage,
                                    .pageNo = state.pageNo});
        bot.answerCallbackQuery(cq.id);
        return;
    }
}

} // namespace cookcookhnya::handlers::recipe
