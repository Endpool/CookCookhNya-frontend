#include "request_history.hpp"

#include "render/personal_account/view.hpp"

namespace cookcookhnya::handlers::personal_account::publication_history {

using namespace render::personal_account;

void handleAllPublicationHistoryCQ(
    AllPublicationHistory& /**/, CallbackQueryRef cq, BotRef& bot, SMRef stateManager, ApiClientRef /**/) {
    const std::string data = cq.data;
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    if (data == "back") {
        renderPersonalAccountMenu(userId, chatId, bot);
        stateManager.put(PersonalAccountMenu{});
        return;
    }
}

} // namespace cookcookhnya::handlers::personal_account::publication_history
