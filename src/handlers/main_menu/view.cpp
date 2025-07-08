#include "view.hpp"

#include "backend/api/storages.hpp"
#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/personal_account/view.hpp"
#include "render/recipes_suggestion/storage_selection/select.hpp"
#include "render/recipes_suggestion/suggest.hpp"
#include "render/shopping_list/view.hpp"
#include "render/storage_list/view.hpp"

#include <vector>

namespace cookcookhnya::handlers::main_menu_view {

using namespace render::storage_list;
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
        renderShoppingList(userId, chatId, bot, api);
        stateManager.put(ShoppingListView{});
        return;
    }
    if (cq.data == "personal_account") {
        renderPersonalAccountMenu(userId, chatId, bot);
        stateManager.put(PersonalAccountMenu{});
        return;
    }
}

} // namespace cookcookhnya::handlers::main_menu_view
