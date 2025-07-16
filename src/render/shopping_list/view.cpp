#include "view.hpp"

#include "message_tracker.hpp"
#include "render/common.hpp"
#include "states.hpp"
#include "utils/to_string.hpp"
#include "utils/utils.hpp"

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <ranges>
#include <utility>

namespace cookcookhnya::render::shopping_list {

void renderShoppingList(const states::ShoppingListView::ItemsDb::Set& items, UserId userId, ChatId chatId, BotRef bot) {
    bool anySelected = std::ranges::any_of(items, &states::ShoppingListView::SelectableItem::selected);

    InlineKeyboard keyboard;
    keyboard.reserve(3 + items.size()); // add, remove and/or buy, list (n), back
    auto buttonRowIter = std::back_inserter(keyboard);

    (*buttonRowIter).push_back(makeCallbackButton(u8"Поиск", "search"));
    if (anySelected) {
        buttonRowIter++;
        keyboard[buttonRowIter].push_back(makeCallbackButton(u8"Убрать", "remove"));
        keyboard[buttonRowIter++].push_back(makeCallbackButton(u8"Купить", "buy"));
    }

    for (auto [i, item] : std::views::enumerate(items)) {
        const char* const selectedMark = item.selected ? "[+] " : "[  ] ";
        keyboard[buttonRowIter++].push_back(
            makeCallbackButton(selectedMark + item.name, utils::to_string(item.ingredientId)));
    }
    keyboard[items.size()].push_back(makeCallbackButton(u8"↩️ Назад", "back"));
    auto messageId = message::getMessageId(userId);
    if (messageId) {
        auto text = utils::utf8str(u8"🔖 Ваш список покупок. Нажмите на элемент, чтобы вычеркнуть из списка.");
        bot.editMessageText(text, chatId, *messageId, makeKeyboardMarkup(std::move(keyboard)));
    }
}

} // namespace cookcookhnya::render::shopping_list
