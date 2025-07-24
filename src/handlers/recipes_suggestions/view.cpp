#include "view.hpp"

#include "backend/api/api.hpp"
#include "backend/id_types.hpp"
#include "backend/models/storage.hpp"
#include "handlers/common.hpp"
#include "render/cooking_planning/view.hpp"
#include "render/main_menu/view.hpp"
#include "render/recipes_suggestions/view.hpp"
#include "render/storage/view.hpp"
#include "utils/ingredients_availability.hpp"
#include "utils/parsing.hpp"

#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

namespace cookcookhnya::handlers::recipes_suggestions {

using namespace render::recipes_suggestions;
using namespace render::storage;
using namespace render::cooking_planning;
using namespace render::main_menu;
using namespace api::models::storage;
using namespace std::views;
using namespace std::literals;

void handleSuggestedRecipesListCQ(
    SuggestedRecipesList& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;
    const std::string& data = cq.data;

    if (data == "back") {
        if (auto* prevState = std::get_if<StorageView>(&state.prevState)) {
            renderStorageView(prevState->storageId, userId, chatId, bot, api);
            std::string storageName = api.getStoragesApi().get(userId, prevState->storageId).name;
            stateManager.put(StorageView{prevState->storageId, std::move(storageName)});
        } else if (auto* prevState = std::get_if<SuggestedRecipesList::FromMainMenuData>(&state.prevState)) {
            renderMainMenu(true, std::nullopt, userId, chatId, bot, api);
            stateManager.put(prevState->first);
        } else if (auto* prevState = std::get_if<StoragesSelection>(&state.prevState)) {
            if (auto* prevPrevState = std::get_if<MainMenu>(&prevState->prevState)) {
                renderMainMenu(true, std::nullopt, userId, chatId, bot, api);
                stateManager.put(auto{*prevPrevState});
            }
            throw std::runtime_error{"Unreachable path reached"};
        }
        bot.answerCallbackQuery(cq.id);
        return;
    }

    if (data.starts_with("recipe_")) {
        auto recipeId = utils::parseSafe<api::RecipeId>(std::string_view{data}.substr("recipe_"sv.size()));
        if (!recipeId)
            return;

        std::vector<CookingPlanning::IngredientAvailability> inStorage =
            utils::inStoragesAvailability(state.getStorageIds(), *recipeId, userId, api);

        renderCookingPlanning(inStorage, *recipeId, userId, chatId, bot, api);
        stateManager.put(CookingPlanning{.prevState = std::move(state),
                                         .addedStorages = {},
                                         .availability = std::move(inStorage),
                                         .recipeId = *recipeId});
        return;
    }

    if (data != "dont_handle") {
        if (data == "page_left")
            state.pageNo--;
        else if (data == "page_right")
            state.pageNo++;
        renderRecipesSuggestion(state.getStorageIds(), state.pageNo, userId, chatId, bot, api);
        return;
    }
}

} // namespace cookcookhnya::handlers::recipes_suggestions
