#include "publish.hpp"

#include "backend/api/publicity_filter.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils/to_string.hpp"
#include "utils/utils.hpp"

#include <cstddef>
#include <format>
#include <utility>

namespace cookcookhnya::render::personal_account::ingredients {

using namespace tg_types;

void renderCustomIngredientPublication(UserId userId, ChatId chatId, BotRef bot, IngredientsApiRef api) {
    auto ingredientsResp = api.search(userId, PublicityFilterType::Custom);

    // TODO: make pagination for ingredients
    auto ingredients = ingredientsResp.page;
    const std::size_t buttonRows = ((ingredients.size() + 1) / 2) + 1;
    InlineKeyboard keyboard(buttonRows);
    for (std::size_t i = 0; i < ingredients.size(); i++) {
        if (i % 2 == 0)
            keyboard[(i / 2)].reserve(2);
        keyboard[(i / 2)].push_back(
            makeCallbackButton("• " + ingredients[i].name, utils::to_string(ingredients[i].id)));
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
