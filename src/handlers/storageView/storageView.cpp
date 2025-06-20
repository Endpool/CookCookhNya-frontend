#include "storageView.hpp"
#include "../../render/ingredientsView/ingredientsRender.hpp"
#include "../../render/storageList/storageListRender.hpp"
#include "../../render/storageView/storageMembersRender.hpp"

namespace handlers::storageView {
using namespace cookcookhnya::forHandlers;
using namespace render::viewIngredients;
using namespace render::viewStorageMembers;
using namespace render::storageList;

void storageViewButtonCallback(StorageView& state, CallbackQueryRef cq, BotRef bot, SMRef stateManager, BackendApiRef api) {
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;
    if (cq.data == "explore") {
        //stateManager.put(IngredientsView{state.storageId}); temporarily not available
        renderIngredientsList(state.storageId, userId, chatId, bot);
    } else if (cq.data == "members") {
        stateManager.put(StorageMemberView{state.storageId});
        renderMemberList(state.storageId, userId, chatId, bot, api);
    } else if (cq.data == "back") {
        stateManager.put(StorageList{});
        renderStorageList(userId, chatId, bot, api);
    }
}
}  // namespace handlers::storageView
