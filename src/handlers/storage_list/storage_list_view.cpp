#include "storage_list_view.hpp"

#include "handlers/common.hpp"
#include "render/storage_list/create_storage_render.hpp"
#include "render/storage_list/delete_storage_render.hpp"
#include "render/storage_view/storage_view_render.hpp"

namespace cookcookhnya::handlers::storage_list_view {

using namespace render::create_storage;
using namespace render::delete_storage;
using namespace render::storage;

void storageListButtonCallback(
    StorageList& /*unused*/, CallbackQueryRef cq, BotRef& bot, SMRef stateManager, StorageApiRef storageApi) {
    bot.answerCallbackQuery(cq.id);

    std::stringstream temp; // Convert string to int
    temp << cq.data;
    int id = 0;
    temp >> id;

    auto chatId = cq.message->chat->id;
    if (cq.data == "StorageViewCreate") {
        stateManager.put(StorageCreationEnterName{}); // Go to function create storage, while cancel button is handled
                                                      // on cancel storage creation
        renderStorageCreate(chatId, bot);             // Bot here prints menu of storage creation ADD USER ID
        return;
    }

    if (cq.data == "StorageViewDelete") {
        stateManager.put(StorageDeletionEnterName{});
        renderStorageDelete(
            chatId, bot, cq.from->id, storageApi); // Need for api, so it could put the list of storages to delete
        return;
    }

    stateManager.put(StorageView{id});
    renderStorageView(
        id, cq.from->id, chatId, bot, storageApi); // If nor buttons were pressed then user pressed on their storages
}

} // namespace cookcookhnya::handlers::storage_list_view
