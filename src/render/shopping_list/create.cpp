#include "create.hpp"

#include "backend/api/ingredients.hpp"
#include "backend/id_types.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils/to_string.hpp"
#include "utils/utils.hpp"

#include <cstddef>
#include <cstdint>
#include <format>
#include <string>
#include <utility>
#include <vector>

namespace cookcookhnya::render::shopping_list {

std::vector<api::IngredientId> renderShoppingListCreation(const std::vector<api::IngredientId>& ingredientIds,
                                                          UserId userId,
                                                          ChatId chatId,
                                                          BotRef bot,
                                                          api::IngredientsApi ingredientsApi) {

    std::string text = utils::utf8str(u8"Основываясь на недостающих ингредиентах, составили для вас продукты "
                                      u8"которые можно добавить в список покупок:\n *В самом низу выберите "
                                      u8"ингредиенты которые вы хотите исключить из списка покупок\n");
    std::vector<std::string> ingredientsName;
    for (const api::IngredientId ingredientId : ingredientIds) {
        // IMPORTANT!: Probably can be optimized because this data is available at the recipe page
        // by Maxim Fomin
        // (1) I believe that both ways are expensive: or it's run through string of textGen or it's several small
        // queries to backend. While i understand that working with string is faster then sending such queries i think
        // that it's better not to overengineering frontend in this aspect.
        //
        // (2) Besides this also lays on frontend additional work on maintaining ingredientsName vector (in this case
        // deletion from it).
        // by Ilia Kliantsevich
        std::string name = ingredientsApi.get(userId, ingredientId).name;
        ingredientsName.push_back(name);
        text += std::format("- {}\n", name);
    }
    const std::size_t buttonRows = ((ingredientIds.size() + 1) / 2) + 2; // +1 for back, +1 for approve

    InlineKeyboard keyboard(buttonRows);
    uint64_t i = 0;
    for (auto ingredientId : ingredientIds) {
        const std::string& name = ingredientsName[i];
        if (i % 2 == 0) {
            keyboard[(i / 2)].reserve(2);
        }
        keyboard[i / 2].push_back(
            makeCallbackButton(name, "i" + utils::to_string(ingredientId))); // i stands for ingredient
        i++;
    }
    keyboard[((ingredientIds.size() + 1) / 2)].push_back(makeCallbackButton(u8"▶️ Подтвердить", "confirm"));
    keyboard[(((ingredientIds.size() + 1) / 2) + 1)].push_back(makeCallbackButton(u8"↩️ Назад", "back"));
    auto messageId = message::getMessageId(userId);
    if (messageId)
        bot.editMessageText(text, chatId, *messageId, makeKeyboardMarkup(std::move(keyboard)));
    return ingredientIds;
}
} // namespace cookcookhnya::render::shopping_list
