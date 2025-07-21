#include "view.hpp"

#include "backend/api/api.hpp"
#include "backend/models/storage.hpp"
#include "handlers/common.hpp"
#include "render/recipes_suggestions/view.hpp"
#include "render/storage/delete.hpp"
#include "render/storage/ingredients/view.hpp"
#include "render/storage/members/view.hpp"
#include "render/storages_list/view.hpp"
#include "states.hpp"

#include <ranges>
#include <utility>
#include <vector>

namespace cookcookhnya::handlers::storage {

using namespace render::storage::ingredients;
using namespace render::storage::members;
using namespace render::storages_list;
using namespace render::recipes_suggestions;
using namespace render::delete_storage;
using namespace api::models::storage;
using namespace std::views;

void handleStorageViewCQ(
    StorageView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    if (cq.data == "ingredients") {
        auto ingredients = api.getIngredientsApi().getStorageIngredients(userId, state.storageId);
        auto newState = StorageIngredientsList{state.storageId, ingredients | as_rvalue, ""};
        renderIngredientsListSearch(newState, userId, chatId, bot);
        stateManager.put(std::move(newState));
        return;
    }

    if (cq.data == "members") {
        renderMemberList(true, state.storageId, userId, chatId, bot, api);
        stateManager.put(StorageMemberView{state.storageId});
        return;
    }

    if (cq.data == "back") {
        renderStorageList(true, userId, chatId, bot, api);
        stateManager.put(StorageList{});
        return;
    }

    if (cq.data == "wanna_eat") {
        auto storageDetails = api.getStoragesApi().get(userId, state.storageId);
        const StorageSummary storage = {.id = state.storageId, .name = storageDetails.name};
        std::vector storages = {storage};
        renderRecipesSuggestion(storages, 0, userId, chatId, bot, api);
        stateManager.put(SuggestedRecipesList{.selectedStorages = storages, .pageNo = 0, .fromStorage = true});
        return;
    }

    if (cq.data == "delete") {
        renderStorageDeletion(state.storageId, chatId, bot, cq.from->id, api);
        stateManager.put(StorageDeletion{state.storageId});
        return;
    }
}

} // namespace cookcookhnya::handlers::storage
