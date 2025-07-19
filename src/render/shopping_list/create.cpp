#include "create.hpp"

#include "backend/models/ingredient.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils/to_string.hpp"
#include "utils/utils.hpp"

#include <cstddef>
#include <format>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

namespace cookcookhnya::render::shopping_list {

using namespace api::models::ingredient;
using namespace std::views;

void renderShoppingListCreation(const std::vector<Ingredient>& selectedIngredients,
                                const std::vector<Ingredient>& allIngredients,
                                UserId userId,
                                ChatId chatId,
                                BotRef bot) {
    std::string text = utils::utf8str(u8"📝 Выберите продукты, которые хотели бы добавить в список покупок\n\n");

    const std::size_t buttonRows = ((selectedIngredients.size() + 1) / 2) + 1; // ceil(ingredientsCount / 2), back
    InlineKeyboardBuilder keyboard{buttonRows};

    for (auto chunk : allIngredients | chunk(2)) {
        keyboard.reserveInRow(2);
        for (const Ingredient& ing : chunk) {
            const bool isSelected = std::ranges::contains(selectedIngredients, ing.id, &api::models::ingredient::Ingredient::id);
            std::string emoji = utils::utf8str(isSelected ? u8"[ + ]" : u8"[ᅠ]");
            const char* actionPrefix = isSelected ? "+" : "-";
            std::string text = std::format("{} {}", emoji, ing.name);
            std::string data = actionPrefix + utils::to_string(ing.id);
            keyboard << makeCallbackButton(text, data);
        }
        keyboard << NewRow{};
    }

    keyboard << makeCallbackButton(u8"↩️ Назад", "back");
    if (!selectedIngredients.empty())
        keyboard << makeCallbackButton(u8"▶️ Подтвердить", "confirm");

    if (auto messageId = message::getMessageId(userId))
        bot.editMessageText(text, chatId, *messageId, std::move(keyboard), "MarkdownV2");
}
} // namespace cookcookhnya::render::shopping_list
