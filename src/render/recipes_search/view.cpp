#include "view.hpp"

#include "backend/models/recipe.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "render/pagination.hpp"
#include "states.hpp"
#include "utils/utils.hpp"

#include <cstddef>
#include <string>

namespace cookcookhnya::render::recipes_search {

using namespace api::models::recipe;

void renderRecipesSearch(const states::helpers::Pagination& pagination,
                         const decltype(states::RecipesSearch::page)& page,
                         UserId userId,
                         ChatId chatId,
                         BotRef bot) {
    const std::string text = utils::utf8str(u8"Используйте кнопку ниже, чтобы искать рецепты");

    InlineKeyboardBuilder keyboard{2 + page.size()};

    auto searchButton = std::make_shared<TgBot::InlineKeyboardButton>();
    searchButton->text = utils::utf8str(u8"🔎 Поиск");
    searchButton->switchInlineQueryCurrentChat = "";
    keyboard << std::move(searchButton) << NewRow{};

    auto makeRecipeButton = [](const RecipeSummary& r) {
        return makeCallbackButton(utils::utf8str(u8"🔖 ") + r.name, "recipe_" + utils::to_string(r.id));
    };
    const std::size_t pageSize = 5;
    keyboard << constructPagination(pagination.pageNo, pageSize, pagination.totalItems, page, makeRecipeButton);

    keyboard << makeCallbackButton(u8"↩️ Назад", "back");

    if (auto mMessageId = message::getMessageId(userId)) {
        bot.editMessageText(text, chatId, *mMessageId, std::move(keyboard));
    } else {
        auto message = bot.sendMessage(chatId, text, std::move(keyboard));
        message::addMessageId(userId, message->messageId);
    }
}

} // namespace cookcookhnya::render::recipes_search
