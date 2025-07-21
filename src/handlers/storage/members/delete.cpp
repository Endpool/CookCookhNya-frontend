#include "delete.hpp"

#include "backend/api/storages.hpp"
#include "handlers/common.hpp"
#include "render/storage/members/view.hpp"
#include "tg_types.hpp"
#include "utils/parsing.hpp"

namespace cookcookhnya::handlers::storage::members {

using namespace render::storage::members;

void handleStorageMemberDeletionCQ(
    StorageMemberDeletion& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, api::StorageApiRef storageApi) {
    bot.answerCallbackQuery(cq.id);

    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    if (cq.data.starts_with("mem_")) {
        auto memberId = utils::parseSafe<tg_types::UserId>(cq.data.substr(4));
        if (memberId) {
            storageApi.deleteMember(userId, state.storageId, *memberId);
        }
    }
    renderMemberList(true, state.storageId, userId, chatId, bot, storageApi);
    stateManager.put(StorageMemberView{state.storageId});
};

} // namespace cookcookhnya::handlers::storage::members
