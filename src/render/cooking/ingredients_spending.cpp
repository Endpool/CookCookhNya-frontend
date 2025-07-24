#include "ingredients_spending.hpp"

#include "message_tracker.hpp"
#include "render/common.hpp"
#include "states.hpp"
#include "utils/utils.hpp"

#include <ranges>
#include <string>

namespace cookcookhnya::render::cooking {

using namespace std::views;
using SelectableIngedient = states::helpers::SelectableShoppingListItem;

void renderIngredientsSpending(const std::vector<states::helpers::SelectableIngredient>& ingredients,
                               bool canRemove,
                               UserId userId,
                               ChatId chatId,
                               BotRef bot) {
    const std::string text = utils::utf8str(
        canRemove ? u8"🧾 Вы можете убрать из хранилища продукты, если они закончились после готовки, а также сразу "
                    u8"добавить их в список покупок"
                  : u8"🧾 Вы можете добавить закончившиеся после готовки продукты в список покупок");

    const bool anySelected = std::ranges::any_of(ingredients, &states::helpers::SelectableIngredient::selected);

    InlineKeyboardBuilder keyboard{2 + ((ingredients.size() / 2) + 1)}; // remove and/or buy, list (n/2), back

    if (anySelected) {
        if (canRemove)
            keyboard << makeCallbackButton(u8"🗑 Убрать", "remove");
        keyboard << makeCallbackButton(u8"🛒 В список покупок", "to_shopping_list") << NewRow{};
    }

    for (auto row : ingredients | chunk(2)) {
        for (const auto& ing : row) {
            const char8_t* const selectedMark = ing.selected ? u8"[ + ] " : u8"[ᅠ] "; // not empty!
            keyboard << makeCallbackButton(utils::utf8str(selectedMark) + ing.name,
                                           "ingredient_" + utils::to_string(ing.id));
        }
        keyboard << NewRow{};
    }

    keyboard << makeCallbackButton(u8"↩️ Назад", "back");

    if (auto messageId = message::getMessageId(userId)) {
        bot.editMessageText(text, chatId, *messageId, std::move(keyboard));
    } else {
        auto message = bot.sendMessage(chatId, text, std::move(keyboard));
        message::addMessageId(userId, message->messageId);
    }
}

} // namespace cookcookhnya::render::cooking
