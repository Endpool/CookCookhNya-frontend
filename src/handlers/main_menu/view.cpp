#include "view.hpp"

#include "backend/api/storages.hpp"
#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/personal_account/view.hpp"
#include "render/recipes_suggestion/storage_selection/select.hpp"
#include "render/recipes_suggestion/suggest.hpp"
#include "render/shopping_list/view.hpp"
#include "render/storage_list/view.hpp"
#include <iterator>

#include <vector>

namespace cookcookhnya::handlers::main_menu {

using namespace render::storages_list;
using namespace render::recipes_suggestion;
using namespace render::select_storages;
using namespace render::shopping_list;
using namespace render::personal_account;

void mainMenuHandler(MainMenu& /*unused*/, CallbackQueryRef cq, BotRef& bot, SMRef stateManager, ApiClientRef api) {
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
            auto storageId = {storages[0].id};
            editRecipesSuggestion(storageId, 0, userId, chatId, bot, api);
            stateManager.put(SuggestedRecipeList{.pageNo = 0, .storageIds = storageId, .fromStorage = false});
            return;
        }
        renderStorageSelect({}, userId, chatId, bot, api);
        stateManager.put(StorageSelection{.storageIds = std::vector<api::StorageId>{}});
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
