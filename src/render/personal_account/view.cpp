#include "view.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils/utils.hpp"

#include <cstddef>
#include <utility>

namespace cookcookhnya::render::personal_account {

using namespace tg_types;

void renderPersonalAccountMenu(UserId userId, ChatId chatId, BotRef bot) {
    const std::size_t buttonRows = 4;
    InlineKeyboard keyboard(buttonRows);

    keyboard[0].push_back(makeCallbackButton(u8"📋 Мои ингредиенты", "ingredients"));
    keyboard[1].push_back(makeCallbackButton(u8"📒 Мои рецепты", "recipes")); // 🗃️
    keyboard[2].push_back(makeCallbackButton(u8"🗃️ История запросов на публикацию", "history"));
    keyboard[3].push_back(makeCallbackButton(u8"↩️ Назад", "back"));

    auto text = utils::utf8str(u8"👤 Вы находитесь в Личном Кабинете. Здесь вы можете добавить личные ингредиенты и "
                               u8"рецепты, а также делиться ими с другими пользователями.");

    auto messageId = message::getMessageId(userId);
    if (messageId) {
        bot.editMessageText(text, chatId, *messageId, makeKeyboardMarkup(std::move(keyboard)));
    } else {
        auto message = bot.sendMessage(chatId, text, makeKeyboardMarkup(std::move(keyboard)));
        message::addMessageId(userId, message->messageId);
    }
}

} // namespace cookcookhnya::render::personal_account
