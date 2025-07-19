#include "add_storage.hpp"

#include "backend/id_types.hpp"
#include "backend/models/storage.hpp"
#include "handlers/common.hpp"
#include "render/recipe/add_storage.hpp"
#include "render/recipe/view.hpp"
#include "states.hpp"
#include "utils/ingredients_availability.hpp"
#include "utils/parsing.hpp"

#include <string>

namespace cookcookhnya::handlers::recipe {

using namespace render::recipe;

void handleRecipeStorageAdditionCQ(
    RecipeStorageAddition& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    std::string data = cq.data;
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    if (data[0] == '-') {
        auto newStorageIdStr = data.substr(1, data.size());
        auto newStorageId = utils::parseSafe<api::StorageId>(newStorageIdStr);
        if (newStorageId) {
            auto newStorageDetails = api.getStoragesApi().get(userId, *newStorageId);
            api::models::storage::StorageSummary newStorage = {.id = *newStorageId, .name = newStorageDetails.name};
            state.addedStorages.push_back(newStorage);
            utils::addStorage(state.availability, newStorage);
            renderStoragesSuggestion(state.availability,
                                     state.selectedStorages,
                                     state.addedStorages,
                                     state.recipeId,
                                     userId,
                                     chatId,
                                     bot,
                                     api);
        }
    }
    if (data[0] == '+') {
        auto newStorageIdStr = data.substr(1, data.size());
        auto newStorageId = utils::parseSafe<api::StorageId>(newStorageIdStr);
        if (newStorageId) {
            auto newStorageDetails = api.getStoragesApi().get(userId, *newStorageId);
            api::models::storage::StorageSummary newStorage = {.id = *newStorageId, .name = newStorageDetails.name};
            state.addedStorages.erase(
                std::ranges::find(state.addedStorages, newStorageId, &api::models::storage::StorageSummary::id));
            utils::deleteStorage(state.availability, newStorage);
            renderStoragesSuggestion(state.availability,
                                     state.selectedStorages,
                                     state.addedStorages,
                                     state.recipeId,
                                     userId,
                                     chatId,
                                     bot,
                                     api);
        }
    }

    if (data == "back") {
        renderRecipeView(state.availability, state.recipeId, userId, chatId, bot, api);
        stateManager.put(RecipeView{.selectedStorages = state.selectedStorages,
                                    .addedStorages = state.addedStorages,
                                    .availability = state.availability,
                                    .recipeId = state.recipeId,
                                    .fromStorage = state.fromStorage,
                                    .pageNo = state.pageNo});
        return;
    }
}

} // namespace cookcookhnya::handlers::recipe
