#include "list.hpp"

#include "render/common.hpp"
#include "utils.hpp"

#include <cstddef>
#include <string>
#include <utility>

namespace cookcookhnya::render::shopping_list {

void renderShoppingList(UserId userId, ChatId chatId, BotRef bot, ShoppingListApiRef api) {
    auto items = api.get(userId);
    InlineKeyboard keyboard(1 + items.size());
    for (std::size_t i = 0; i < items.size(); i++) {
        keyboard[i].push_back(detail::makeCallbackButton(items[i].name, std::to_string(items[i].ingredientId)));
    }
    keyboard[items.size()].push_back(detail::makeCallbackButton(u8"Назад", "back"));
    bot.sendMessage(chatId,
                    utils::utf8str(u8"Ваш список покупок:"),
                    nullptr,
                    nullptr,
                    detail::makeKeyboardMarkup(std::move(keyboard)));
}

} // namespace cookcookhnya::render::shopping_list
