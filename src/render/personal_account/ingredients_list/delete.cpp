#include "delete.hpp"

#include "backend/models/ingredient.hpp"
#include "backend/models/publication_request_status.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils/to_string.hpp"
#include "utils/utils.hpp"

#include <format>
#include <ranges>
#include <utility>
#include <vector>

namespace cookcookhnya::render::personal_account::ingredients {

using namespace std::views;

void renderCustomIngredientDeletion(UserId userId, ChatId chatId, BotRef bot, IngredientsApiRef api) {
    auto ingredientsResp = api.customIngredientsSearch(userId, "", 0);

    // TODO: make pagination for ingredients
    std::vector<api::models::ingredient::CustomIngredient> ingredients;
    for (const auto& ing : ingredientsResp.page) {
        if (ing.moderationStatus == api::models::moderation::PublicationRequestStatus::NO_REQUEST) {
            ingredients.push_back(ing);
        }
    }

    InlineKeyboardBuilder keyboard{ingredients.size() + 1};
    for (auto chunk : ingredients | chunk(2)) {
        keyboard.reserveInRow(2);
        for (const auto& i : chunk) {
            keyboard << makeCallbackButton("• " + i.name, utils::to_string(i.id));
        }
        keyboard << NewRow{};
    }

    keyboard << makeCallbackButton(u8"↩️ Назад", "back");

    auto text = std::format("{} Какой ингредиент вы хотите удалить?\n", utils::utf8str(u8"🚮"));
    auto messageId = message::getMessageId(userId);
    if (messageId) {
        bot.editMessageText(text, chatId, *messageId, std::move(keyboard));
    }
}

} // namespace cookcookhnya::render::personal_account::ingredients
