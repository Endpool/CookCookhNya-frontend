#include "view.hpp"

#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/main_menu/view.hpp"
#include "render/recipe/view.hpp"
#include "render/recipes_suggestions/view.hpp"
#include "render/storage/view.hpp"
#include "render/storages_selection/view.hpp"
#include "utils/ingredients_availability.hpp"
#include "utils/parsing.hpp"

#include <string>
#include <utility>

namespace cookcookhnya::handlers::recipes_suggestions {

using namespace render::recipes_suggestions;
using namespace render::select_storages;
using namespace render::storage;
using namespace render::recipe;
using namespace render::main_menu;

void handleSuggestedRecipeListCQ(
    SuggestedRecipeList& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    auto data = cq.data;

    if (data == "back") {
        if (state.fromStorage) {
            renderStorageView(state.storages[0].id, cq.from->id, chatId, bot, api);
            stateManager.put(StorageView{state.storages[0].id}); // Go to the only one storage
        } else {
            if (api.getStoragesApi().getStoragesList(userId).size() == 1) {
                renderMainMenu(true, userId, chatId, bot, api);
                stateManager.put(MainMenu{});
            } else {
                renderStorageSelection(state.storages, userId, chatId, bot, api);
                stateManager.put(StoragesSelection{.storages = std::move(state.storages)});
            }
        }
        bot.answerCallbackQuery(cq.id);
        return;
    }

    if (data[0] == 'r') {
        auto recipeId = utils::parseSafe<api::RecipeId>(data.substr(1, data.size()));
        if (recipeId) {
            auto inStorage = utils::inStoragesAvailability(state.storages, *recipeId, userId, api);
            renderRecipeView(inStorage, *recipeId, userId, chatId, bot, api);
            stateManager.put(RecipeView{.storages = state.storages,
                                        .availability = inStorage,
                                        .recipeId = *recipeId,
                                        .fromStorage = state.fromStorage,
                                        .pageNo = state.pageNo});
        }
        return;
    }

    if (data != "dont_handle") {
        auto pageNo = utils::parseSafe<std::size_t>(data);
        if (pageNo) {
            state.pageNo = *pageNo;
        }
        // Message is 100% exists as it was rendered by some another method
        renderRecipesSuggestion(state.storages, *pageNo, userId, chatId, bot, api);
        return;
    }
}

} // namespace cookcookhnya::handlers::recipes_suggestions
