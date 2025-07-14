#include "create.hpp"

#include "backend/models/ingredient.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils/utils.hpp"

#include <algorithm>
#include <format>
#include <string>
#include <utility>

namespace cookcookhnya::render::create_custom_ingredient {

void renderCustomIngredientCreate(UserId userId, ChatId chatId, BotRef bot) {
    InlineKeyboard keyboard(1);
    keyboard[0].push_back(makeCallbackButton(u8"↩️ Назад", "back"));
    auto text = utils::utf8str(u8"🥦 Введите новое имя ингредиента");
    auto messageId = message::getMessageId(userId);
    if (messageId) {
        bot.editMessageText(text, chatId, *messageId, "", "", nullptr, makeKeyboardMarkup(std::move(keyboard)));
    }
}

void renderCustomIngredientConfirm(
    std::string ingredientName, UserId userId, ChatId chatId, BotRef bot, IngredientsApiRef api) {
    InlineKeyboard keyboard(2);
    keyboard[0].push_back(makeCallbackButton(u8"▶️ Подтвердить", "confirm"));
    keyboard[1].push_back(makeCallbackButton(u8"↩️ Назад", "back"));

    auto similarIngredients = api.search(std::move(ingredientName), 5, 0, 70).page; // NOLINT(*magic-numbers*)

    std::string text;
    if (!similarIngredients.empty()) {
        std::string formatedIngredients;
        std::ranges::for_each(similarIngredients,
                              [&formatedIngredients](const api::models::ingredient::Ingredient& item) {
                                  formatedIngredients += "• " + item.name + "\n";
                              });
        text = std::format("{}\n{}{}",
                           utils::utf8str(u8"🔍 Нашли похожие ингредиенты:"),
                           formatedIngredients,
                           utils::utf8str(u8"Вы уверены, что хотите добавить новый ингредиент?"));
    } else {
        text = utils::utf8str(u8"Вы уверены, что хотите добавить новый ингредиент?");
    }
    auto message = bot.sendMessage(chatId, text, nullptr, nullptr, makeKeyboardMarkup(std::move(keyboard)));
    message::addMessageId(userId, message->messageId);
}

} // namespace cookcookhnya::render::create_custom_ingredient
