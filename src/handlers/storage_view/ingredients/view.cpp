#include "view.hpp"

#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"
#include "handlers/common.hpp"
#include "message_tracker.hpp"
#include "render/storage_view/ingredients/view.hpp"
#include "render/storage_view/view.hpp"
#include "utils.hpp"

#include <algorithm>
#include <cstddef>
#include <functional>
#include <utility>

namespace cookcookhnya::handlers::storage::ingredients {

using namespace render::storage;
using namespace render::storage::ingredients;
using namespace api::models::ingredient;
using namespace tg_types;

void storageIngredientsSearchButtonCallback(
    StorageIngredientsList& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    const auto userId = cq.from->id;
    const auto chatId = cq.message->chat->id;

    if (cq.data == "back") {
        renderStorageView(state.storageId, userId, chatId, bot, api);
        stateManager.put(StorageView{state.storageId});
        return;
    }

    auto mIngredient = utils::parseSafe<api::IngredientId>(cq.data);
    if (!mIngredient)
        return;
    auto it = std::ranges::find(state.searchItems, *mIngredient, &IngredientSearchItem::id);
    if (it == state.searchItems.end())
        return;

    if (it->available) {
        api.getIngredientsApi().deleteFromStorage(userId, state.storageId, *mIngredient);
        state.removeIngredient(*mIngredient);
    } else {
        api.getIngredientsApi().putToStorage(userId, state.storageId, *mIngredient);
        state.putIngredient({it->id, it->name});
    }
    it->available = !it->available;

    renderIngredientsListSearch(state, userId, chatId, bot);
}

void storageIngredientsSearchInlineQueryCallback(StorageIngredientsList& state,
                                                 InlineQueryRef iq,
                                                 BotRef bot,
                                                 IngredientsApiRef api) {
    const auto userId = iq.from->id;
    if (iq.query.empty()) {
        state.searchItems.clear();
        renderIngredientsListSearch(state, userId, userId, bot);
    } else {
        const std::size_t count = 100;
        auto response = api.search(userId, iq.query, state.storageId, count, 0);
        if (response.found != state.totalFound || !std::ranges::equal(response.page,
                                                                      state.searchItems,
                                                                      std::ranges::equal_to{},
                                                                      &IngredientSearchItem::id,
                                                                      &IngredientSearchItem::id)) {
            state.searchItems = std::move(response.page);
            state.totalFound = response.found;
            if (auto mMessageId = message::getMessageId(userId))
                renderIngredientsListSearch(state, userId, userId, bot);
        }
    }
    // Cache is not disabled on Windows and Linux desktops. Works on Android and Web
    bot.answerInlineQuery(iq.id, {}, 0);
}

} // namespace cookcookhnya::handlers::storage::ingredients
