#include "view.hpp"

#include "backend/api/publicity_filter.hpp"
#include "backend/models/ingredient.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils/utils.hpp"

#include <algorithm>
#include <cstddef>
#include <format>
#include <string>
#include <utility>

namespace cookcookhnya::render::personal_account::ingredients {

using namespace tg_types;

void renderCustomIngredientsList(bool toBeEdited, UserId userId, ChatId chatId, BotRef bot, IngredientsApiRef api) {
    auto ingredientsResp = api.getList(userId, PublicityFilterType::Custom);
    auto ingredients = ingredientsResp.page;
    const std::size_t buttonRows = ingredients.empty() ? 2 : 3;
    InlineKeyboard keyboard(buttonRows);

    if (ingredients.empty()) {
        keyboard[0].push_back(makeCallbackButton(u8"🆕 Создать", "create"));
        keyboard[1].push_back(makeCallbackButton(u8"↩️ Назад", "back"));
    } else {
        keyboard[0].push_back(makeCallbackButton(u8"🆕 Создать", "create"));
        keyboard[1].push_back(makeCallbackButton(u8"📢 Опубликовать", "publish"));
        keyboard[2].push_back(makeCallbackButton(u8"↩️ Назад", "back"));
    }

    std::string formatedIngredients;
    std::ranges::for_each(ingredients, [&formatedIngredients](const api::models::ingredient::Ingredient& item) {
        formatedIngredients += "• " + item.name + "\n";
    });

    auto text = std::format("{} Вы находитесь в Мои ингредиенты. \nВами созданные ингредиенты:\n{}",
                            utils::utf8str(u8"📋"),
                            formatedIngredients);

    if (toBeEdited) {
        if (auto messageId = message::getMessageId(userId))
            bot.editMessageText(text, chatId, *messageId, makeKeyboardMarkup(std::move(keyboard)));

    } else {
        auto message = bot.sendMessage(chatId, text, makeKeyboardMarkup(std::move(keyboard)));
        message::addMessageId(userId, message->messageId);
    }
}

} // namespace cookcookhnya::render::personal_account::ingredients
