#include "view.hpp"
#include "message_tracker.hpp"

namespace cookcookhnya::render::personal_account {

void renderPersonalAccountMenu(UserId userId, ChatId chatId, BotRef bot) {
    auto text = utils::utf8str(u8"Ваш личный кабинет");
    InlineKeyboard keyboard(2);
    keyboard[0].reserve(2);
    keyboard[0].push_back(detail::makeCallbackButton(u8"Ингредиенты", "custom_ingredients"));
    keyboard[0].push_back(detail::makeCallbackButton(u8"Рецепты", "custom_recipes"));
    keyboard[1].push_back(detail::makeCallbackButton(u8"Назад", "back"));
    auto messageId = message::getMessageId(userId);
    if (messageId) {
        bot.editMessageText(text, chatId, *messageId, "", "", nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
    }
}

} // namespace cookcookhnya::render::personal_account
