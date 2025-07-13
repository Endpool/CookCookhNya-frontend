#include "publish.hpp"

#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils.hpp"

#include <cstddef>
#include <format>
#include <string>
#include <utility>

namespace cookcookhnya::render::personal_account::ingredients {

using namespace tg_types;

void renderCustomIngredientPublish(UserId userId, ChatId chatId, BotRef bot, IngredientsApiRef api) {
    auto ingredients = api.getCustomIngredients(userId);

    const std::size_t buttonRows = ((ingredients.size() + 1) / 2) + 1;
    InlineKeyboard keyboard(buttonRows);
    for (std::size_t i = 0; i < ingredients.size(); i++) {
        if (i % 2 == 0)
            keyboard[(i / 2)].reserve(2);
        keyboard[(i / 2)].push_back(makeCallbackButton("• " + ingredients[i].name, std::to_string(ingredients[i].id)));
    }

    keyboard[buttonRows - 1].push_back(makeCallbackButton(u8"↩️ Назад", "back"));

    auto text = std::format(
        "{} Какой ингредиент вы хотите предложить для добавления в CookCookNya? (Все предложения проходят проверку)\n ",
        utils::utf8str(u8"📥"));
    auto messageId = message::getMessageId(userId);
    if (messageId) {
        bot.editMessageText(text, chatId, *messageId, "", "", nullptr, makeKeyboardMarkup(std::move(keyboard)));
    }
}

} // namespace cookcookhnya::render::personal_account::ingredients
