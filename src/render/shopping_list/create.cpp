#include "create.hpp"

#include "backend/models/ingredient.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils/to_string.hpp"
#include "utils/utils.hpp"

#include <cstddef>
#include <format>
#include <string>
#include <utility>
#include <vector>

namespace cookcookhnya::render::shopping_list {

void renderShoppingListCreation(std::vector<api::models::ingredient::Ingredient> selectedIngredients,
                                UserId userId,
                                ChatId chatId,
                                BotRef bot) {
    std::string text =
        std::format("{} Выберите продукты, которые хотели бы добавить в список покупок\n\n", utils::utf8str(u8"📝"));
    const size_t buttonRows = ((selectedIngredients.size() + 1) / 2) + 1;
    InlineKeyboard keyboard(buttonRows);

    for (std::size_t i = 0; i < selectedIngredients.size(); ++i) {
        if (i % 2 == 0)
            keyboard[i / 2].reserve(2);
        const bool isSelected =
            std::ranges::find(selectedIngredients,
                              selectedIngredients[i].id,
                              &api::models::ingredient::Ingredient::id) != selectedIngredients.end();
        std::string emoji = utils::utf8str(isSelected ? u8"[ + ]" : u8"[ᅠ]");
        const char* actionPrefix = isSelected ? "+" : "-";
        const std::string text = std::format("{} {}", emoji, selectedIngredients[i].name);
        const std::string data = actionPrefix + utils::to_string(selectedIngredients[i].id);
        keyboard[i / 2].push_back(makeCallbackButton(text, data));
    }
    keyboard[buttonRows - 1].push_back(makeCallbackButton(u8"▶️ Подтвердить", "confirm"));
    if (!selectedIngredients.empty()) {
        keyboard[buttonRows - 1].push_back(makeCallbackButton(u8"↩️ Назад", "back"));
    }
    auto messageId = message::getMessageId(userId);
    if (messageId) {
        bot.editMessageText(text, chatId, *messageId, makeKeyboardMarkup(std::move(keyboard)), "MarkdownV2");
    }
}
} // namespace cookcookhnya::render::shopping_list
