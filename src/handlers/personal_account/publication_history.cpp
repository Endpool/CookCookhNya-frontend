#include "publication_history.hpp"

#include "render/personal_account/view.hpp"

namespace cookcookhnya::handlers::personal_account {

using namespace render::personal_account;

void handleTotalPublicationHistoryCQ(
    TotalPublicationHistory& /*unused*/, CallbackQueryRef cq, BotRef& bot, SMRef stateManager, api::ApiClientRef /**/) {
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

} // namespace cookcookhnya::handlers::personal_account
