#include "search.hpp"

#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"
#include "handlers/common.hpp"
#include "render/storage_view/ingredients/list.hpp"
#include "render/storage_view/ingredients/search.hpp"
#include "utils.hpp"

#include <algorithm>
#include <functional>

namespace cookcookhnya::handlers::storage::ingredients {

using namespace api::models::ingredient;
using namespace render::storage::ingredients;

void storageIngredientsSearchButtonCallback(
    StorageIngredientsSearch& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, IngredientsApiRef api) {
    bot.answerCallbackQuery(cq.id);
    const auto user = cq.from->id;
    if (cq.data == "back") {
        renderIngredientsList(state.storageId, user, user, bot, api);
        stateManager.put(StorageIngredientsList{state.storageId});
        return;
    }

    auto mIngredient = utils::parseSafe<api::IngredientId>(cq.data);
    if (!mIngredient)
        return;
    auto it = std::ranges::find(state.shownIngredients, *mIngredient, &IngredientSearchResult::id);
    if (it == state.shownIngredients.end())
        return;
    if (it->available)
        api.deleteFromStorage(user, state.storageId, *mIngredient);
    else
        api.putToStorage(user, state.storageId, *mIngredient);
    it->available = !it->available;

    renderStorageIngredientsSearchEdit(state.shownIngredients, state.message, user, bot);
}

void storageIngredientsSearchInlineQueryCallback(StorageIngredientsSearch& state,
                                                 InlineQueryRef iq,
                                                 BotRef bot,
                                                 IngredientsApiRef api) {
    if (!iq.query.empty()) {
        const auto user = iq.from->id;
        auto newIngredients = api.search(user, iq.query, state.storageId);
        if (!std::ranges::equal(newIngredients,
                                state.shownIngredients,
                                std::equal_to<>{},
                                &IngredientSearchResult::id,
                                &IngredientSearchResult::id)) {
            state.shownIngredients = newIngredients;
            renderStorageIngredientsSearchEdit(state.shownIngredients, state.message, user, bot);
        }
    }
    // Cache is not disabled on Windows and Linux desktops. Works on Android and Web
    bot.answerInlineQuery(iq.id, {}, 0);
}

} // namespace cookcookhnya::handlers::storage::ingredients
