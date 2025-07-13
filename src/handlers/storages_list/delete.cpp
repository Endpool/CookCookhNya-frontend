#include "delete.hpp"

#include "backend/id_types.hpp"
#include "handlers/common.hpp"
#include "render/storages_list/view.hpp"
#include "utils.hpp"

namespace cookcookhnya::handlers::storages_list {

using namespace render::storages_list;

void deleteStorage(
    StorageDeletion& /*unused*/, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi) {
    bot.answerCallbackQuery(cq.id);
    if (cq.data.starts_with("st")) {
        auto storageId = utils::parseSafe<api::StorageId>(cq.data.substr(4));
        if (storageId) {
            storageApi.delete_(cq.from->id, *storageId);
        }
    }
    renderStorageList(true, cq.from->id, cq.message->chat->id, bot, storageApi);
    stateManager.put(StorageList{});
};

} // namespace cookcookhnya::handlers::storages_list
