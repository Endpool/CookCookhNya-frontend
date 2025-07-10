#include "create.hpp"

#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils.hpp"

#include <utility>

namespace cookcookhnya::render::create_custom_recipe {

void renderRecipeCreate(ChatId chatId, UserId userId, BotRef bot) { // BackendProvider bkn
    InlineKeyboard keyboard(1);
    keyboard[0].push_back(makeCallbackButton(u8"🚫 Отмена", "cancel_recipe_creation"));
    auto text = utils::utf8str(u8"🏷 Введите новое имя рецепта");
    auto messageId = message::getMessageId(userId);
    if (messageId) {
        bot.editMessageText(text, chatId, *messageId, "", "", nullptr, makeKeyboardMarkup(std::move(keyboard)));
    }
};

} // namespace cookcookhnya::render::create_custom_recipe
