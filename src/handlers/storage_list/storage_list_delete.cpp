#include "storage_list_delete.hpp"

#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/storage_list/storage_list_render.hpp"
#include "utils.hpp"

namespace cookcookhnya::handlers::storage_delete {

using namespace render::storage_list;

void deleteStorage(
    StorageDeletion& /*unused*/, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi) {
    bot.answerCallbackQuery(cq.id);
    if (cq.data.starts_with("st")) {
        auto storageId = utils::parseSafe<api::StorageId>(cq.data.substr(4));
        storageApi.delete_(cq.from->id, *storageId);
    }
    renderStorageList(true, cq.from->id, cq.message->chat->id, bot, storageApi);
    stateManager.put(StorageList{});
};

} // namespace cookcookhnya::handlers::storage_delete
