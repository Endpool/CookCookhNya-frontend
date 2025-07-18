#include "delete.hpp"

#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/storages_list/view.hpp"
#include "render/storage/view.hpp"
#include "states.hpp"

namespace cookcookhnya::handlers::storages_list {

using namespace render::storages_list;
using namespace render::storage;

void handleStorageDeletionCQ(
    StorageDeletion& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi) {
    bot.answerCallbackQuery(cq.id);
    if (cq.data == "confirm") {
        storageApi.delete_(cq.from->id, state.storageId);
        renderStorageList(true, cq.from->id, cq.message->chat->id, bot, storageApi);
        stateManager.put(StorageList{});
    }
    if (cq.data == "back"){
        renderStorageView(state.storageId, cq.from->id, cq.message->chat->id, bot, storageApi);
        stateManager.put(StorageView{state.storageId});
    }
    
};

} // namespace cookcookhnya::handlers::storages_list
