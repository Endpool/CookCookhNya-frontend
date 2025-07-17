#include "view.hpp"

#include "message_tracker.hpp"
#include "render/common.hpp"
#include "states.hpp"
#include "utils/to_string.hpp"
#include "utils/utils.hpp"

#include <ranges>
#include <utility>

namespace cookcookhnya::render::shopping_list {

void renderShoppingList(const states::ShoppingListView::ItemsDb::Set& items, UserId userId, ChatId chatId, BotRef bot) {
    InlineKeyboard keyboard(1 + items.size());
    for (auto [i, item] : std::views::enumerate(items))
        keyboard[i].push_back(makeCallbackButton(item.name, utils::to_string(item.ingredientId)));
    keyboard[items.size()].push_back(makeCallbackButton(u8"↩️ Назад", "back"));
    auto messageId = message::getMessageId(userId);
    if (messageId) {
        auto text = utils::utf8str(u8"🔖 Ваш список покупок. Нажмите на элемент, чтобы вычеркнуть из списка.");
        bot.editMessageText(text, chatId, *messageId, "", "", nullptr, makeKeyboardMarkup(std::move(keyboard)));
    }
}

} // namespace cookcookhnya::render::shopping_list
