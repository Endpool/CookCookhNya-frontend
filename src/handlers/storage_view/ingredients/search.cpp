#include "search.hpp"

#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/storage_view/ingredients/list.hpp"
#include "render/storage_view/ingredients/search.hpp"
#include "utils.hpp"

namespace cookcookhnya::handlers::storage::ingredients {

void storageIngredientsSearchButtonCallback(
    StorageIngredientsSearch& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    auto user = cq.from->id;
    auto chat = cq.from->id;
    if (cq.data == "back") {
        render::storage::ingredients::renderIngredientsList(state.storageId, user, chat, bot, api);
        stateManager.put(StorageIngredientsList{state.storageId});
        return;
    }

    if (cq.data.empty())
        return;
    const char action = cq.data[0];
    auto mIngredient = utils::parseSafe<api::IngredientId>(cq.data.substr(1));
    if (!mIngredient)
        return;

    switch (action) {
    case '+':
        api.getIngredients().putToStorage(user, state.storageId, *mIngredient);
        break;
    case '-':
        api.getIngredients().deleteFromStorage(user, state.storageId, *mIngredient);
        break;
    default:
        return;
    }
    render::storage::ingredients::renderStorageIngredientsSearchEdit(state.message, chat, bot, api);
}

} // namespace cookcookhnya::handlers::storage::ingredients
