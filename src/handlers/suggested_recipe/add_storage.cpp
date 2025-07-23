#include "add_storage.hpp"

#include "backend/api/api.hpp"
#include "backend/id_types.hpp"
#include "backend/models/storage.hpp"
#include "handlers/common.hpp"
#include "render/suggested_recipe/add_storage.hpp"
#include "render/suggested_recipe/view.hpp"
#include "states.hpp"
#include "utils/ingredients_availability.hpp"
#include "utils/parsing.hpp"

#include <algorithm>
#include <string>
#include <string_view>
#include <utility>

namespace cookcookhnya::handlers::suggested_recipe {

using namespace render::suggested_recipe;
using namespace api::models::storage;

void handleRecipeStorageAdditionCQ(
    RecipeStorageAddition& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    const std::string& data = cq.data;
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    if (data == "back") {
        renderRecipeView(state.prevState.availability, state.prevState.recipeId, userId, chatId, bot, api);
        stateManager.put(auto{std::move(state.prevState)});
        return;
    }

    if (data[0] == '-') {
        auto newStorageId = utils::parseSafe<api::StorageId>(std::string_view{data}.substr(1));
        if (newStorageId) {
            auto newStorageDetails = api.getStoragesApi().get(userId, *newStorageId);
            const StorageSummary newStorage = {.id = *newStorageId, .name = newStorageDetails.name};
            state.prevState.addedStorages.push_back(newStorage);
            utils::addStorage(state.prevState.availability, newStorage);
            renderStoragesSuggestion(state.prevState.availability,
                                     state.prevState.prevState.selectedStorages,
                                     state.prevState.addedStorages,
                                     state.prevState.recipeId,
                                     userId,
                                     chatId,
                                     bot,
                                     api);
        }
    }

    if (data[0] == '+') {
        auto newStorageId = utils::parseSafe<api::StorageId>(std::string_view{data}.substr(1));
        if (newStorageId) {
            auto newStorageDetails = api.getStoragesApi().get(userId, *newStorageId);
            const StorageSummary newStorage = {.id = *newStorageId, .name = newStorageDetails.name};
            state.prevState.addedStorages.erase(std::ranges::find(
                state.prevState.addedStorages, newStorageId, &api::models::storage::StorageSummary::id));
            utils::deleteStorage(state.prevState.availability, newStorage);
            renderStoragesSuggestion(state.prevState.availability,
                                     state.prevState.prevState.selectedStorages,
                                     state.prevState.addedStorages,
                                     state.prevState.recipeId,
                                     userId,
                                     chatId,
                                     bot,
                                     api);
        }
    }
}

} // namespace cookcookhnya::handlers::suggested_recipe
