#include "view.hpp"

#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"
#include "handlers/common.hpp"
#include "render/recipe/add_storage.hpp"
#include "render/recipes_suggestions/view.hpp"
#include "render/shopping_list/create.hpp"

#include <string>
#include <utility>
#include <vector>

namespace cookcookhnya::handlers::recipe {

using namespace render::recipes_suggestions;
using namespace render::shopping_list;
using namespace render::recipe;

void handleRecipeViewCQ(RecipeView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    std::string data = cq.data;
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    if (data == "start_cooking") {
        // TODO: add state of begginig of cooking
        return;
    }

    if (data == "shopping_list") {
        std::vector<api::models::ingredient::Ingredient> selectedIngredients;
        std::vector<api::models::ingredient::Ingredient> allIngredients;
        for (const auto& infoPair : state.availability) {
            if (infoPair.second.available == utils::AvailabiltiyType::NOT_AVAILABLE) {
                selectedIngredients.push_back({.id = infoPair.first.id, .name = infoPair.first.name});
            }
            allIngredients.push_back({.id = infoPair.first.id, .name = infoPair.first.name});
        }
        renderShoppingListCreation(selectedIngredients, allIngredients, userId, chatId, bot);
        stateManager.put(ShoppingListCreation{
            .prevState = std::move(state),
            .selectedIngredients = selectedIngredients,
            .allIngredients = allIngredients,
        });
        bot.answerCallbackQuery(cq.id);
        return;
    }

    if (data == "back_from_recipe_view") {
        renderRecipesSuggestion(state.prevState.selectedStorages, state.prevState.pageNo, userId, chatId, bot, api);
        stateManager.put(auto{std::move(state.prevState)});
        bot.answerCallbackQuery(cq.id);
        return;
    }

    if (data == "add_storages") {
        renderStoragesSuggestion(state.availability,
                                 state.prevState.selectedStorages,
                                 state.addedStorages,
                                 state.recipeId,
                                 userId,
                                 chatId,
                                 bot,
                                 api);
        stateManager.put(RecipeStorageAddition{.prevState = std::move(state)});
        return;
    }
}

} // namespace cookcookhnya::handlers::recipe
