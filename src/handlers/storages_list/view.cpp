#include "view.hpp"

#include "backend/api/storages.hpp"
#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/main_menu/view.hpp"
#include "render/storage/view.hpp"
#include "render/storages_list/create.hpp"
#include "utils/parsing.hpp"

#include <optional>

namespace cookcookhnya::handlers::storages_list {

using namespace render::main_menu;
using namespace render::create_storage;
using namespace render::storage;

void handleStorageListCQ(
    StorageList& /*unused*/, CallbackQueryRef cq, BotRef& bot, SMRef stateManager, ApiClientRef api) {
    bot.answerCallbackQuery(cq.id);
    auto userId = cq.from->id;
    auto chatId = cq.message->chat->id;
    auto storages = api.getStoragesApi().getStoragesList(userId);
    if (cq.data == "create") {
        renderStorageCreation(chatId, userId, bot);
        stateManager.put(StorageCreationEnterName{});
        return;
    }
    if (cq.data == "back") {
        renderMainMenu(true, std::nullopt, userId, chatId, bot, api);
        stateManager.put(MainMenu{});
        return;
    }
    auto storageId = utils::parseSafe<api::StorageId>(cq.data);
    if (storageId) {
        renderStorageView(*storageId, cq.from->id, chatId, bot, api);
        stateManager.put(StorageView{*storageId});
    }
}

} // namespace cookcookhnya::handlers::storages_list
