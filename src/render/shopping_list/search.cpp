#include "search.hpp"

#include "backend/models/ingredient.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "render/pagination.hpp"
#include "states.hpp"

#include <cstddef>
#include <string>
#include <utility>

namespace TgBot {
class InlineKeyboardButton;
} // namespace TgBot

namespace cookcookhnya::render::shopping_list {

using namespace api::models::ingredient;
using namespace tg_types;
using namespace std::views;
using namespace std::ranges;

void renderShoppingListIngredientSearch(
    const states::ShoppingListIngredientSearch& state, std::size_t pageSize, UserId userId, ChatId chatId, BotRef bot) {
    std::string text = utils::utf8str(u8"🍗 Используйте кнопку ниже, чтобы найти ингредиенты для добавления");

    InlineKeyboardBuilder keyboard{state.page.size() + 2}; // search, items (n), back

    auto searchButton = std::make_shared<TgBot::InlineKeyboardButton>();
    searchButton->text = utils::utf8str(u8"🔍 Искать");
    searchButton->switchInlineQueryCurrentChat = "";
    keyboard << std::move(searchButton) << NewRow{};

    auto makeItemButton = [](const Ingredient& ing) {
        // const auto* emptyBrackets = "[ㅤ] ";
        return makeCallbackButton(ing.name, "ingredient_" + utils::to_string(ing.id));
    };
    keyboard << constructPagination(
                    state.pagination.pageNo, pageSize, state.pagination.totalItems, state.page, makeItemButton)
             << makeCallbackButton(u8"↩️ Назад", "back");

    if (auto messageId = message::getMessageId(userId))
        bot.editMessageText(text, chatId, *messageId, std::move(keyboard));
}

} // namespace cookcookhnya::render::shopping_list
