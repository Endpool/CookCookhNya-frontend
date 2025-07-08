#include "handlers/personal_account/view.hpp"

#include "handlers/common.hpp"
#include "render/custom_recipes_list/view.hpp"
#include "render/personal_account/view.hpp"

namespace cookcookhnya::handlers::personal_account_view {
using namespace render::custom_recipes_list;
using namespace render::personal_account;

void personalAccount(PersonalAccountMenu& /**/, CallbackQueryRef cq, BotRef bot, SMRef stateManager, ApiClientRef api) {
    std::string data = cq.data;
    bot.answerCallbackQuery(cq.id);
    auto chatId = cq.message->chat->id;
    auto userId = cq.from->id;

    if (data == "custom_recipes") {
        renderCustomRecipesList(0, userId, chatId, bot, api);
        stateManager.put(CustomRecipesList{.pageNo = 0});
    }
    if (data == "back") {
        renderPersonalAccountMenu(userId, chatId, bot);
        stateManager.put(PersonalAccountMenu{});
        return;
    }
    if (data == "custom_ingredients") {
        // render custom ingredients
    }
}

} // namespace cookcookhnya::handlers::personal_account_view
