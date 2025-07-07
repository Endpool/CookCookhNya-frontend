#include "create.hpp"

#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils.hpp"

namespace cookcookhnya::render::create_custom_ingredient {

void renderCustomIngredientCreate(UserId userId, ChatId chatId, BotRef bot) {
    InlineKeyboard keyboard(1);
    keyboard[0].push_back(detail::makeCallbackButton(u8"🚫 Отмена", "cancel"));
    auto text = utils::utf8str(u8"🌽 Введите новое имя ингредиента");
    auto messageId = message::getMessageId(userId);
    if (messageId) {
        bot.editMessageText(text, chatId, *messageId, "", "", nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
    }
}

void renderCustomIngredientConfirm(
    std::string ingredientName, UserId userId, ChatId chatId, BotRef bot, IngredientsApiRef api) {
    InlineKeyboard keyboard(2);
    keyboard[0].push_back(detail::makeCallbackButton(u8"🚫 Отмена", "cancel"));
    keyboard[0].push_back(detail::makeCallbackButton(u8"✅ Подтвердить", "confirm"));

    auto similarIngredients = api.getAllIngredients(ingredientName, 5, 0, 70); // NOLINT
    std::string formatedIngredients;
    std::ranges::for_each(similarIngredients, [&formatedIngredients](const api::models::ingredient::Ingredient& item) {
        formatedIngredients += "• " + item.name + "\n";
    });
    auto text = std::format("{} Нашли похожие ингредиенты:\n{}Вы уверены, что хотите добавить новый ингредиент?",
                            utils::utf8str(u8"🔍"),
                            formatedIngredients);
    auto message = bot.sendMessage(chatId, text, nullptr, nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
    message::addMessageId(userId, message->messageId);
}

} // namespace cookcookhnya::render::create_custom_ingredient
