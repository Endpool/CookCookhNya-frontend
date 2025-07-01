#pragma once

#include "recipe_view.hpp"
#include "backend/id_types.hpp"
#include "render/recipes_suggestion/recipe_view_render.hpp"
#include "render/recipes_suggestion/recipes_suggestion_render.hpp"

namespace {
std::vector<cookcookhnya::api::StorageId> mySplit(const std::string& input) {
    std::istringstream iss(input);
    std::vector<cookcookhnya::api::StorageId> result;

    // Using istream_iterator to read integers directly from the string stream
    std::copy(std::istream_iterator<cookcookhnya::api::StorageId>(iss),
              std::istream_iterator<cookcookhnya::api::StorageId>(),
              std::back_inserter(result));

    return result;
}
} // namespace

namespace cookcookhnya::handlers::recipe_view {
using namespace render::recipe_view;
using render::recipes_suggestion::renderRecipesSuggestion;

void handleRecipeView(RecipeView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {

    std::string data = cq.data;
    auto messageId = cq.message->messageId;

    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    if (data == "startCooking") {
        // TODO: add state of begginig of cooking
        return;
    }
    if (data == "makeReceipt") {
        // TODO: add state of rec
        return;
    }
    if (data == "backFromRecipeView") {
        renderRecipesSuggestion(state.storageIds, 1, userId, chatId, bot, api);
        stateManager.put(SuggestedRecipeList{.pageNo = 1,
                                             .storageIds = std::move(state.storageIds),
                                             .fromStorage = state.storageIds.size() ==
                                                            0}); // I don't know if it came from storage or no
        return;
    }
    if (data == "BackFromAddingStorages") {
        renderRecipeViewAfterAddingStorage(state.storageIds, state.recipeId, userId, chatId, messageId, bot, api);
    }
    if (data[0] == '?') {
        // In state we have storages which were chosen by user to suggest recipes -
        // not storages ingredients from which are suggested
        renderStorageSuggestion(state.storageIds, state.recipeId, userId, chatId, messageId, bot, api);
        return;
    }
    if (data[0] == '+') {
        auto storageIds = mySplit(data.substr(1, data.size()));
        state.storageIds.push_back(storageIds[storageIds.size() - 1]);
        storageIds.pop_back();
        renderStorageSuggestion(state.storageIds, state.recipeId, userId, chatId, messageId, bot, api);
    }
}

} // namespace cookcookhnya::handlers::recipe_view
