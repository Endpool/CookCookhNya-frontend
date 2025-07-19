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

void handleSuggestedRecipesListCQ(
    SuggestedRecipesList& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    auto data = cq.data;

    if (data == "back") {
        if (state.fromStorage) {
            renderStorageView(state.selectedStorages[0].id, userId, chatId, bot, api);
            stateManager.put(StorageView{state.selectedStorages[0].id}); // Go to the only one storage
        } else {
            if (api.getStoragesApi().getStoragesList(userId).size() == 1) {
                renderMainMenu(true, userId, chatId, bot, api);
                stateManager.put(MainMenu{});
            } else {
                auto newState = StoragesSelection{.selectedStorages = std::move(state.selectedStorages)};
                renderStorageSelection(newState, userId, chatId, bot, api);
                stateManager.put(std::move(newState));
            }
        }
        bot.answerCallbackQuery(cq.id);
        return;
    }

    if (data.starts_with("recipe_")) {
        auto recipeId = utils::parseSafe<api::RecipeId>(data.substr(sizeof("recipe_") - 1));
        if (!recipeId)
            return;
        auto inStorage = utils::inStoragesAvailability(state.selectedStorages, *recipeId, userId, api);
        renderRecipeView(inStorage, *recipeId, userId, chatId, bot, api);
        stateManager.put(RecipeView{
            .prevState = std::move(state),
            .addedStorages = {},
            .availability = inStorage,
            .recipeId = *recipeId,
        });
        return;
    }

    if (data != "dont_handle") {
        auto pageNo = utils::parseSafe<std::size_t>(data);
        if (pageNo)
            state.pageNo = *pageNo;

        renderRecipesSuggestion(state.selectedStorages, state.pageNo, userId, chatId, bot, api);
        return;
    }
}

} // namespace cookcookhnya::handlers::recipes_suggestions
