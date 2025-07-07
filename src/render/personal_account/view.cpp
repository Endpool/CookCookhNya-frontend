#include "view.hpp"

#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils.hpp"

#include <cstddef>
#include <string>
#include <utility>

namespace cookcookhnya::render::personal_account {

using namespace tg_types;

void renderPersonalAccountMenu(UserId userId, ChatId chatId, BotRef bot) {

    const std::size_t buttonRows = 3;
    InlineKeyboard keyboard(buttonRows);

    keyboard[0].push_back(detail::makeCallbackButton(u8"🌽 Мои ингредиенты", "my_ingredients"));
    keyboard[1].push_back(detail::makeCallbackButton(u8"📒 Мои рецепты", "my_recipes"));
    keyboard[2].push_back(detail::makeCallbackButton(u8"↩️ Назад", "back"));

    auto text = utils::utf8str(u8"👤 Вы находитесь в Личном Кабинете. Здесь вы можете добавить личные ингредиенты и "
                               u8"рецепты, а также делиться ими с другими пользователями.");

    auto messageId = message::getMessageId(userId);
    if (messageId) {
        bot.editMessageText(text, chatId, *messageId, "", "", nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
    }
}

} // namespace cookcookhnya::render::personal_account
