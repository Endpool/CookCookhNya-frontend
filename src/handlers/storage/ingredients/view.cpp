#include "view.hpp"

#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"
#include "handlers/common.hpp"
#include "message_tracker.hpp"
#include "render/storage/ingredients/view.hpp"
#include "render/storage/view.hpp"
#include "tg_types.hpp"
#include "utils/parsing.hpp"

#include <algorithm>
#include <cstddef>
#include <functional>
#include <utility>

namespace cookcookhnya::handlers::storage::ingredients {

using namespace render::storage;
using namespace render::storage::ingredients;
using namespace api::models::ingredient;

// Global vars
const size_t numOfIngredientsOnPage = 5;
const size_t threshhold = 70;

namespace {
void updateSearch(
    StorageIngredientsList& state, bool isQueryChanged, BotRef bot, tg_types::UserId userId, IngredientsApiRef api) {
    state.pageNo = isQueryChanged ? 0 : state.pageNo;
    auto response = api.searchForStorage(userId,
                                         state.storageId,
                                         state.inlineQuery,
                                         threshhold,
                                         numOfIngredientsOnPage,
                                         state.pageNo * numOfIngredientsOnPage);
    if (response.found != state.totalFound || !std::ranges::equal(response.page,
                                                                  state.searchItems,
                                                                  std::ranges::equal_to{},
                                                                  &IngredientSearchForStorageItem::id,
                                                                  &IngredientSearchForStorageItem::id)) {
        state.searchItems = std::move(response.page);
        state.totalFound = response.found;
        if (auto mMessageId = message::getMessageId(userId))
            renderIngredientsListSearch(state, numOfIngredientsOnPage, userId, userId, bot);
    }
}
} // namespace

void handleStorageIngredientsListCQ(
    StorageIngredientsList& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    const auto userId = cq.from->id;
    const auto chatId = cq.message->chat->id;

    if (cq.data == "back") {
        renderStorageView(state.storageId, userId, chatId, bot, api);
        stateManager.put(StorageView{state.storageId});
        return;
    }
    if (cq.data == "prev") {
        state.pageNo -= 1;
        updateSearch(state, false, bot, userId, api);
        return;
    }

    if (cq.data == "next") {
        state.pageNo += 1;
        updateSearch(state, false, bot, userId, api);
        return;
    }

    if (cq.data != "dont_handle") {

        auto mIngredient = utils::parseSafe<api::IngredientId>(cq.data);
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
        renderIngredientsListSearch(state, numOfIngredientsOnPage, userId, chatId, bot);
    }
}

void handleStorageIngredientsListIQ(StorageIngredientsList& state,
                                    InlineQueryRef iq,
                                    BotRef bot,
                                    IngredientsApiRef api) {
    const auto userId = iq.from->id;
    state.inlineQuery = iq.query;
    if (iq.query.empty()) {
        state.searchItems.clear();
        // When query is empty then search shouldn't happen
        state.totalFound = 0;
        renderIngredientsListSearch(state, numOfIngredientsOnPage, userId, userId, bot);
    } else {
        updateSearch(state, true, bot, userId, api);
    }
    // Cache is not disabled on Windows and Linux desktops. Works on Android and Web
    // bot.answerInlineQuery(iq.id, {}, 0);
}

} // namespace cookcookhnya::handlers::storage::ingredients
