#include "storage_delete_member.hpp"

#include "handlers/common.hpp"
#include "render/common.hpp"
#include "render/storage_view/storage_members_render.hpp"
#include "utils.hpp"

namespace cookcookhnya::handlers::storage_delete_member {

using namespace render::storage::member_list;
using namespace render;

void deleteMember(
    MemberDeletion& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, StorageApiRef storageApi) {
    bot.answerCallbackQuery(cq.id);

    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    if (cq.data.starts_with("mem_")) {
        auto memberId = utils::parseSafe<UserId>(cq.data.substr(4));
        if (memberId) {
            storageApi.deleteMember(userId, state.storageId, *memberId);
        }
    }
    renderMemberList(true, state.storageId, userId, chatId, bot, storageApi);
    stateManager.put(StorageMemberView{state.storageId});
};

} // namespace cookcookhnya::handlers::storage_delete_member
