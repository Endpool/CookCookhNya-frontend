#include "view.hpp"

#include "backend/api/api.hpp"
#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"
#include "handlers/common.hpp"
#include "message_tracker.hpp"
#include "render/storage/ingredients/delete.hpp"
#include "render/storage/ingredients/view.hpp"

#include "render/personal_account/ingredients_list/create.hpp"

#include "render/storage/view.hpp"
#include "states.hpp"
#include "tg_types.hpp"
#include "utils/parsing.hpp"

#include <algorithm>
#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace cookcookhnya::handlers::storage::ingredients {

using namespace render::storage;
using namespace render::storage::ingredients;
using namespace render::suggest_custom_ingredient;
using namespace render::personal_account::ingredients;
using namespace api::models::ingredient;
using namespace std::literals;

namespace {

const size_t numOfIngredientsOnPage = 5;
const size_t threshhold = 70;

void updateSearch(StorageIngredientsList& state,
                  bool isQueryChanged,
                  BotRef bot,
                  tg_types::UserId userId,
                  api::IngredientsApiRef api) {
    state.pageNo = isQueryChanged ? 0 : state.pageNo;
    auto response = api.searchForStorage(userId,
                                         state.storageId,
                                         state.inlineQuery,
                                         numOfIngredientsOnPage,
                                         state.pageNo * numOfIngredientsOnPage,
                                         threshhold);
    state.totalFound = response.found;
    if (state.totalFound == 0) {
        renderSuggestIngredientCustomisation(state, userId, userId, bot);
        return;
    }
    const auto idGetter = &IngredientSearchForStorageItem::id;
    if (std::ranges::equal(response.page, state.searchItems, {}, idGetter, idGetter))
        return;

    state.searchItems = std::move(response.page);
    state.totalFound = response.found;
    if (auto mMessageId = message::getMessageId(userId))
        renderIngredientsListSearch(state, userId, userId, bot);
}

} // namespace

void handleStorageIngredientsListCQ(
    StorageIngredientsList& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    const auto userId = cq.from->id;
    const auto chatId = cq.message->chat->id;

    if (cq.data == "back") {
        renderStorageView(state.storageId, userId, chatId, bot, api);
        std::string storageName = api.getStoragesApi().get(userId, state.storageId).name;
        stateManager.put(StorageView{state.storageId, std::move(storageName)});
        return;
    }

    if (cq.data == "delete") {
        std::vector<api::models::ingredient::Ingredient> ingredients;
        for (auto& ing : state.storageIngredients.getValues()) {
            ingredients.push_back(ing);
        }
        auto newState = StorageIngredientsDeletion{state.storageId, {}, ingredients, false, 0};
        renderStorageIngredientsDeletion(newState, userId, chatId, bot);
        stateManager.put(newState);
        return;
    }

    if (cq.data == "page_left") {
        state.pageNo -= 1;
        updateSearch(state, false, bot, userId, api);
        return;
    }

    if (cq.data == "page_right") {
        state.pageNo += 1;
        updateSearch(state, false, bot, userId, api);
        return;
    }

    if (cq.data.starts_with("create_ingredient")) {
        const std::string ingredientName = state.inlineQuery;
        renderCustomIngredientConfirmation(true, ingredientName, userId, chatId, bot, api);
        stateManager.put(CustomIngredientConfirmation{ingredientName, std::nullopt, std::nullopt, state.storageId});
    }

    if (cq.data.starts_with("ingredient_")) {
        auto mIngredient =
            utils::parseSafe<api::IngredientId>(std::string_view{cq.data}.substr("ingredient_"sv.size()));
        if (!mIngredient)
            return;
        auto it = std::ranges::find(state.searchItems, *mIngredient, &IngredientSearchForStorageItem::id);
        if (it == state.searchItems.end())
            return;

        if (it->isInStorage) {
            api.getIngredientsApi().deleteFromStorage(userId, state.storageId, *mIngredient);
            state.storageIngredients.remove(*mIngredient);
        } else {
            api.getIngredientsApi().putToStorage(userId, state.storageId, *mIngredient);
            state.storageIngredients.put({.id = it->id, .name = it->name});
        }
        it->isInStorage = !it->isInStorage;
        renderIngredientsListSearch(state, userId, chatId, bot);
    }
}

void handleStorageIngredientsListIQ(StorageIngredientsList& state,
                                    InlineQueryRef iq,
                                    BotRef bot,
                                    api::IngredientsApiRef api) {
    const auto userId = iq.from->id;
    state.inlineQuery = iq.query;
    if (iq.query.empty()) {
        state.searchItems.clear();
        // When query is empty then search shouldn't happen
        state.totalFound = 0;
        renderIngredientsListSearch(state, userId, userId, bot);
    } else {
        updateSearch(state, true, bot, userId, api);
    }
    // Cache is not disabled on Windows and Linux desktops. Works on Android and Web
    // Not answer to disable cache
    // bot.answerInlineQuery(iq.id, {}, 0);
}

} // namespace cookcookhnya::handlers::storage::ingredients
