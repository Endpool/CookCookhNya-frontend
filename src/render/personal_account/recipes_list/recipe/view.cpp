#include "view.hpp"

#include "backend/id_types.hpp"
#include "backend/models/recipe.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils/utils.hpp"

#include <cstddef>
#include <format>
#include <string>
#include <utility>

namespace cookcookhnya::render::personal_account::recipes {

void renderCustomRecipe(
    bool toBeEdited, UserId userId, ChatId chatId, api::RecipeId recipeId, BotRef bot, RecipesApiRef recipesApi) {
    auto recipeDetails = recipesApi.get(userId, recipeId);

    const std::size_t rows = 4; // 1 for publish, 1 for delete, 1 for back, 1 for change
    InlineKeyboard keyboard(rows);
    std::string toPrint;
    toPrint += (utils::utf8str(u8"Рецепт: ") + recipeDetails.name + "\n");
    for (auto& it : recipeDetails.ingredients)
        toPrint += std::format("• {}\n", it.name);
    toPrint += recipeDetails.link;
    keyboard[0].push_back(makeCallbackButton(u8"🚮 Удалить", "delete"));
    keyboard[1].push_back(makeCallbackButton(u8"✏️ Редактировать", "change"));
    keyboard[2].push_back(makeCallbackButton(u8"📢 Опубликовать", "publish"));
    keyboard[3].push_back(makeCallbackButton(u8"↩️ Назад", "back"));

    if (toBeEdited) {
        auto messageId = message::getMessageId(userId);
        if (messageId)
            bot.editMessageText(toPrint, chatId, *messageId, makeKeyboardMarkup(std::move(keyboard)));
    } else {
        auto message = bot.sendMessage(chatId, toPrint, makeKeyboardMarkup(std::move(keyboard)));
        message::addMessageId(userId, message->messageId);
    }
}
} // namespace cookcookhnya::render::personal_account::recipes
