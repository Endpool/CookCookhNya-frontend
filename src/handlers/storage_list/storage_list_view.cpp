#include "storage_list_view.hpp"

#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/recipes_suggestion/select_storages_render.hpp"
#include "render/storage_list/create_storage_render.hpp"
#include "render/storage_list/delete_storage_render.hpp"
#include "render/storage_view/storage_view_render.hpp"
#include "states.hpp"

#include <vector>

namespace cookcookhnya::handlers::storage_list_view {

using namespace render::create_storage;
using namespace render::delete_storage;
using namespace render::select_storages;
using namespace render::storage;

void storageListButtonCallback(
    StorageList& /*unused*/, CallbackQueryRef cq, BotRef& bot, SMRef stateManager, StorageApiRef storageApi) {
    bot.answerCallbackQuery(cq.id);

    std::stringstream temp; // Convert string to int
    temp << cq.data;
    int id = 0;
    temp >> id;

    auto chatId = cq.message->chat->id;
    if (cq.data == "storage_list_creation") {
        renderStorageCreate(chatId, bot);
        stateManager.put(StorageCreationEnterName{}); // Go to function create storage, while cancel button is handled
                                                      // on cancel storage creation
        return;
    }

    if (cq.data == "storage_list_deletion") {
        renderStorageDelete(
            chatId, bot, cq.from->id, storageApi); // Need for api, so it could put the list of storages to delete
        stateManager.put(StorageDeletionEnterName{});
        return;
    }
    if (cq.data == "storage_list_what_to_cook") {
        auto messageId = renderStoragesSelect(cq.from->id, chatId, bot, storageApi);
        stateManager.put(StorageSelection{.storageIds = std::vector<api::StorageId>{}, .messageId = messageId});
        return;
    }
    renderStorageView(
        id, cq.from->id, chatId, bot, storageApi); // If nor buttons were pressed then user pressed on their storages
    stateManager.put(StorageView{id});
}

} // namespace cookcookhnya::handlers::storage_list_view
