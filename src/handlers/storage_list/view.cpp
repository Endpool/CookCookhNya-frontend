#include "view.hpp"

#include "backend/api/storages.hpp"
#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/main_menu/main_menu_render.hpp"
#include "render/storage_list/create_storage_render.hpp"
#include "render/storage_list/delete_storage_render.hpp"
#include "render/storage_view/storage_view_render.hpp"
#include "utils.hpp"

#include <optional>

namespace cookcookhnya::handlers::storage_list_view {

using namespace render::main_menu;
using namespace render::create_storage;
using namespace render::delete_storage;
using namespace render::storage;

void storageListButtonCallback(
    StorageList& /*unused*/, CallbackQueryRef cq, BotRef& bot, SMRef stateManager, ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
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
    if (cq.data == "back") {
        renderMainMenu(true, userId, chatId, bot, api);
        stateManager.put(MainMenu{});
        return;
    }
    auto storageId = utils::parseSafe<api::StorageId>(cq.data);
    if (storageId) {
        renderStorageView(*storageId, cq.from->id, chatId, bot, api);
        stateManager.put(StorageView{*storageId});
    }
}

} // namespace cookcookhnya::handlers::storage_list_view
