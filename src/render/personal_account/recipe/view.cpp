#include "view.hpp"

#include "backend/api/recipes.hpp"
#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"
#include "backend/models/publication_request_status.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils/utils.hpp"

#include <cstddef>
#include <format>
#include <string>
#include <utility>
#include <vector>

namespace cookcookhnya::render::personal_account::recipe {

using namespace api::models::ingredient;
using namespace api::models::moderation;

std::pair<std::vector<Ingredient>, std::string> renderCustomRecipe(
    bool toBeEdited, UserId userId, ChatId chatId, api::RecipeId recipeId, BotRef bot, api::RecipesApiRef recipesApi) {
    auto recipeDetails = recipesApi.get(userId, recipeId);

    std::vector<api::models::ingredient::Ingredient> ingredients;

    const std::size_t rows = 4; // 1 for publish, 1 for delete, 1 for back, 1 for change
    InlineKeyboardBuilder keyboard{rows};
    std::string toPrint;
    toPrint += std::format("{} Ингредиенты для *{}* \n\n", utils::utf8str(u8"📖"), recipeDetails.name);

    for (auto& it : recipeDetails.ingredients) {
        toPrint += std::format("• {}\n", it.name);
        ingredients.push_back({
            .id = it.id,
            .name = it.name,
        });
    }

    toPrint += "\n🌐 [Статус проверки] " + utils::to_string(recipeDetails.moderationStatus);

    keyboard << makeCallbackButton(u8"🚮 Удалить", "delete") << NewRow{};
    keyboard << makeCallbackButton(u8"✏️ Редактировать", "change") << NewRow{};
    // Show publish button only iff the status is not emty AND not rejected
    if (recipeDetails.moderationStatus == PublicationRequestStatus::NO_REQUEST ||
        recipeDetails.moderationStatus == PublicationRequestStatus::REJECTED) {
        keyboard << makeCallbackButton(u8"📢 Опубликовать", "publish") << NewRow{};
    } else {
        keyboard << makeCallbackButton(u8"📢 История публикаций", "publish") << NewRow{};
    }

    keyboard << makeCallbackButton(u8"↩️ Назад", "back");

    if (toBeEdited) {
        auto messageId = message::getMessageId(userId);
        if (messageId)
            bot.editMessageText(toPrint, chatId, *messageId, std::move(keyboard), "Markdown");
    } else {
        auto message = bot.sendMessage(chatId, toPrint, std::move(keyboard), "Markdown");
        message::addMessageId(userId, message->messageId);
    }
    return {std::move(ingredients), std::move(recipeDetails.name)};
}

} // namespace cookcookhnya::render::personal_account::recipe
