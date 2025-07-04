#include "storage_list_view.hpp"

#include "backend/api/storages.hpp"
#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/recipes_suggestion/recipes_suggestion_render.hpp"
#include "render/recipes_suggestion/select_storages_render.hpp"
#include "render/shopping_list/list.hpp"
#include "render/storage_list/create_storage_render.hpp"
#include "render/storage_list/delete_storage_render.hpp"
#include "render/storage_view/storage_view_render.hpp"

#include <sstream>
#include <vector>

namespace cookcookhnya::handlers::storage_list_view {

using namespace render::create_storage;
using namespace render::delete_storage;
using namespace render::select_storages;
using namespace render::shopping_list;
using namespace render::storage;
using namespace render::recipes_suggestion;

void storageListButtonCallback(
    StorageList& /*unused*/, CallbackQueryRef cq, BotRef& bot, SMRef stateManager, ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    std::stringstream temp; // Convert string to int
    temp << cq.data;
    int id = 0;
    temp >> id;
    auto userId = cq.from->id;
    auto chatId = cq.message->chat->id;
    auto storages = api.getStoragesApi().getStoragesList(userId);
    if (cq.data == "storage_list_creation") {
        renderStorageCreate(chatId, userId, bot);
        stateManager.put(StorageCreationEnterName{});
        return;
    }

    if (cq.data == "storage_list_deletion") {
        renderStorageDelete(chatId, bot, cq.from->id, api);
        stateManager.put(StorageDeletion{});
        return;
    }
    if (cq.data == "storage_list_what_to_cook") {
        if (storages.size() == 1) {
            auto storageId = {storages[0].id};
            editRecipesSuggestion(storageId, 1, userId, chatId, bot, api);
            stateManager.put(SuggestedRecipeList{.pageNo = 1, .storageIds = storageId, .fromStorage = true});
            return;
        }
        renderStorageSelect({}, cq.from->id, chatId, bot, api);
        stateManager.put(StorageSelection{.storageIds = std::vector<api::StorageId>{}});
        return;
    }
    if (cq.data == "shopping_list") {
        renderShoppingList(cq.from->id, chatId, bot, api);
        stateManager.put(ShoppingListView{});
        return;
    }
    renderStorageView(
        id, cq.from->id, chatId, bot, api); // If nor buttons were pressed then user pressed on their storages
    stateManager.put(StorageView{id});
}

} // namespace cookcookhnya::handlers::storage_list_view
