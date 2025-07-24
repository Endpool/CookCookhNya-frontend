#include "view.hpp"

#include "backend/api/api.hpp"
#include "handlers/common.hpp"
#include "render/personal_account/view.hpp"
#include "render/recipes_search/view.hpp"
#include "render/recipes_suggestions/view.hpp"
#include "render/shopping_list/view.hpp"
#include "render/storages_list/view.hpp"
#include "render/storages_selection/view.hpp"

#include <utility>
#include <vector>

namespace cookcookhnya::handlers::main_menu {

using namespace render::storages_list;
using namespace render::recipes_suggestions;
using namespace render::select_storages;
using namespace render::shopping_list;
using namespace render::personal_account;
using namespace render::recipes_search;
using namespace std::views;

void handleMainMenuCQ(MainMenu& state, CallbackQueryRef cq, BotRef& bot, SMRef stateManager, api::ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    auto userId = cq.from->id;
    auto chatId = cq.message->chat->id;
    auto storages = api.getStoragesApi().getStoragesList(userId);

    if (cq.data == "storage_list") {
        renderStorageList(true, userId, chatId, bot, api);
        stateManager.put(StorageList{});
        return;
    }

    if (cq.data == "wanna_eat") {
        if (storages.size() == 1) {
            renderRecipesSuggestion({storages[0].id}, 0, userId, chatId, bot, api);
            stateManager.put(SuggestedRecipesList{
                .prevState = SuggestedRecipesList::FromMainMenuData{state, std::move(storages[0])}, .pageNo = 0});
            return;
        }
        renderStorageSelection({}, userId, chatId, bot, api);
        stateManager.put(StoragesSelection{.prevState = state, .selectedStorages = {}});
        return;
    }

    if (cq.data == "shopping_list") {
        const bool canBuy = !storages.empty();
        auto items = api.getShoppingListApi().get(userId);

        auto newState = ShoppingListView{.items = std::move(items), .canBuy = canBuy};
        renderShoppingList(newState, userId, chatId, bot);
        stateManager.put(std::move(newState));
        return;
    }

    if (cq.data == "recipes_search") {
        auto newState = RecipesSearch{};
        renderRecipesSearch(newState.pagination, newState.page, userId, chatId, bot);
        stateManager.put(std::move(newState));
        return;
    }

    if (cq.data == "personal_account") {
        renderPersonalAccountMenu(userId, chatId, bot);
        stateManager.put(PersonalAccountMenu{});
        return;
    }
}

} // namespace cookcookhnya::handlers::main_menu
