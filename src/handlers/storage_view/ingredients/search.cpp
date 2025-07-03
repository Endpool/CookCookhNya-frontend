#include "search.hpp"

#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"
#include "handlers/common.hpp"
#include "render/storage_view/ingredients/list.hpp"
#include "render/storage_view/ingredients/search.hpp"
#include "utils.hpp"

#include <algorithm>
#include <cstddef>
#include <functional>
#include <utility>

namespace cookcookhnya::handlers::storage::ingredients {

using namespace api::models::ingredient;
using namespace render::storage::ingredients;

void storageIngredientsSearchButtonCallback(
    StorageIngredientsSearch& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, IngredientsApiRef api) {
    bot.answerCallbackQuery(cq.id);
    const auto user = cq.from->id;
    const auto chat = cq.message->chat->id;
    if (cq.data == "back") {
        renderIngredientsList(state.storageId, user, chat, bot, api);
        stateManager.put(StorageIngredientsList{state.storageId});
        return;
    }

    auto mIngredient = utils::parseSafe<api::IngredientId>(cq.data);
    if (!mIngredient)
        return;
    auto it = std::ranges::find(state.shownIngredients, *mIngredient, &IngredientSearchItem::id);
    if (it == state.shownIngredients.end())
        return;
    if (it->available)
        api.deleteFromStorage(user, state.storageId, *mIngredient);
    else
        api.putToStorage(user, state.storageId, *mIngredient);
    it->available = !it->available;

    renderStorageIngredientsSearchEdit(state.shownIngredients, state.pageNo, 1, state.messageId, user, bot);
}

void storageIngredientsSearchInlineQueryCallback(StorageIngredientsSearch& state,
                                                 InlineQueryRef iq,
                                                 BotRef bot,
                                                 IngredientsApiRef api) {
    if (!iq.query.empty()) {
        const auto user = iq.from->id;
        const std::size_t count = 100;
        auto response = api.search(user, iq.query, state.storageId, count, 0);
        if (response.found != state.totalFound || !std::ranges::equal(response.page,
                                                                      state.shownIngredients,
                                                                      std::ranges::equal_to{},
                                                                      &IngredientSearchItem::id,
                                                                      &IngredientSearchItem::id)) {
            state.shownIngredients = std::move(response.page);
            state.totalFound = response.found;
            renderStorageIngredientsSearchEdit(state.shownIngredients, state.pageNo, 1, state.messageId, user, bot);
        }
    }
    // Cache is not disabled on Windows and Linux desktops. Works on Android and Web
    bot.answerInlineQuery(iq.id, {}, 0);
}

} // namespace cookcookhnya::handlers::storage::ingredients
