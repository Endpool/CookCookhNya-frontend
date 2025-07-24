#include "delete.hpp"

#include "backend/api/storages.hpp"
#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/storage/view.hpp"
#include "render/storages_list/view.hpp"
#include "states.hpp"
#include <utility>

namespace cookcookhnya::handlers::storage {

using namespace render::storages_list;
using namespace render::storage;

void handleStorageDeletionCQ(
    StorageDeletion& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::StorageApiRef storageApi) {
    bot.answerCallbackQuery(cq.id);
    auto userId = cq.from->id;
    auto chatId = cq.message->chat->id;

    if (cq.data == "confirm") {
        storageApi.delete_(userId, state.storageId);
        renderStorageList(true, userId, chatId, bot, storageApi);
        stateManager.put(StorageList{});
    }

    if (cq.data == "back") {
        renderStorageView(state.storageId, userId, chatId, bot, storageApi);
        std::string storageName = storageApi.get(userId, state.storageId).name;
        stateManager.put(StorageView{state.storageId, std::move(storageName)});
    }
};

} // namespace cookcookhnya::handlers::storage
