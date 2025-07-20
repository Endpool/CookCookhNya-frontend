#include "view.hpp"

#include "message_tracker.hpp"
#include "render/common.hpp"
#include "states.hpp"
#include "utils/to_string.hpp"
#include "utils/utils.hpp"

#include <algorithm>
#include <ranges>
#include <utility>

namespace cookcookhnya::render::shopping_list {

void renderShoppingList(const states::ShoppingListView& state, UserId userId, ChatId chatId, BotRef bot) {
    auto items = state.items.getValues();
    const bool anySelected = std::ranges::any_of(items, &states::ShoppingListView::SelectableItem::selected);

    InlineKeyboardBuilder keyboard{3 + items.size()}; // add, remove and/or buy, list (n), back

    keyboard << makeCallbackButton(u8"🔍 Поиск", "search") << NewRow{};

    if (anySelected) {
        keyboard << makeCallbackButton(u8"🗑 Убрать", "remove");
        if (state.canBuy)
            keyboard << makeCallbackButton(u8"🛒 Купить", "buy");
        keyboard << NewRow{};
    }

    for (const auto& item : items) {
        const char* const selectedMark = item.selected ? "[ + ] " : "[ᅠ] ";
        keyboard << makeCallbackButton(selectedMark + item.name, utils::to_string(item.ingredientId)) << NewRow{};
    }

    keyboard << makeCallbackButton(u8"↩️ Назад", "back");

    auto text =
        utils::utf8str(u8"🔖 Ваш список покупок. Выбирайте, чтобы добавить в хранилище или вычеркнуть из списка.");
    if (auto messageId = message::getMessageId(userId)) {
        bot.editMessageText(text, chatId, *messageId, std::move(keyboard));
    } else {
        auto message = bot.sendMessage(chatId, text, std::move(keyboard));
        message::addMessageId(userId, message->messageId);
    }
}

} // namespace cookcookhnya::render::shopping_list
