#include "view.hpp"

#include "backend/api/storages.hpp"
#include "backend/models/storage.hpp"
#include "handlers/common.hpp"
#include "render/personal_account/view.hpp"
#include "render/recipes_suggestions/view.hpp"
#include "render/shopping_list/view.hpp"
#include "render/storages_list/view.hpp"
#include "render/storages_selection/view.hpp"

#include <iterator>
#include <vector>

namespace cookcookhnya::handlers::main_menu {

using namespace render::storages_list;
using namespace render::recipes_suggestions;
using namespace render::select_storages;
using namespace render::shopping_list;
using namespace render::personal_account;

void handleMainMenuCQ(MainMenu& /*unused*/, CallbackQueryRef cq, BotRef& bot, SMRef stateManager, ApiClientRef api) {
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
            std::vector<api::models::storage::StorageSummary> storage = {storages[0]};
            renderRecipesSuggestion(storage, 0, userId, chatId, bot, api);
            stateManager.put(SuggestedRecipeList{.pageNo = 0, .selectedStorages = storage, .fromStorage = false});
            return;
        }
        renderStorageSelection({}, userId, chatId, bot, api);
        stateManager.put(StoragesSelection{.selectedStorages = std::vector<api::models::storage::StorageSummary>{}});
        return;
    }
    if (cq.data == "shopping_list") {
        auto items = api.getShoppingListApi().get(userId);
        stateManager.put(
            ShoppingListView{{{std::make_move_iterator(items.begin()), std::make_move_iterator(items.end())}}});
        renderShoppingList(std::get<ShoppingListView>(*stateManager.get()).items.getAll(), userId, chatId, bot);
        return;
    }
    if (cq.data == "personal_account") {
        renderPersonalAccountMenu(userId, chatId, bot);
        stateManager.put(PersonalAccountMenu{});
        return;
    }
}

} // namespace cookcookhnya::handlers::main_menu
