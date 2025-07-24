#include "view.hpp"

#include "backend/api/ingredients.hpp"
#include "backend/models/ingredient.hpp"
#include "backend/models/publication_request_status.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils/utils.hpp"

#include <tgbot/types/InlineKeyboardButton.h>

#include <cmath>
#include <cstddef>
#include <format>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace cookcookhnya::render::personal_account::ingredients {

using namespace api::models::ingredient;
using namespace tg_types;
using namespace cookcookhnya::api::models::moderation;

namespace {

std::pair<std::string, InlineKeyboard>
constructMessage(api::models::ingredient::CustomIngredientList& ingredientsList) { // NOLINT(*complexity*)
    std::size_t numOfRows = 0;
    if (ingredientsList.found == 0)
        numOfRows = 2;
    else
        numOfRows = 4;
    std::string text;
    InlineKeyboard keyboard(numOfRows);

    std::vector<api::models::ingredient::CustomIngredient> noReq;
    std::vector<api::models::ingredient::CustomIngredient> pending;
    std::vector<api::models::ingredient::CustomIngredient> accepted;
    std::vector<api::models::ingredient::CustomIngredient> rejected;
    for (const auto& ing : ingredientsList.page) {
        switch (ing.moderationStatus) {
        case PublicationRequestStatus::NO_REQUEST:
            noReq.push_back(ing);
            break;
        case PublicationRequestStatus::PENDING:
            pending.push_back(ing);
            break;
        case PublicationRequestStatus::ACCEPTED:
            accepted.push_back(ing);
            break;
        case PublicationRequestStatus::REJECTED:
            rejected.push_back(ing);
            break;
        }
    }

    if (ingredientsList.found == 0) {
        text =
            utils::utf8str(u8"📋 Вы находитесь в Мои ингредиенты\\. Создавайте и делитесь новыми ингредиентами\\.\n\n");
        keyboard[0].push_back(makeCallbackButton(u8"🆕 Создать", "create"));
        keyboard[1].push_back(makeCallbackButton(u8"↩️ Назад", "back"));
    } else {
        text = utils::utf8str(u8"📋 Вы находитесь в Мои ингредиенты\\. \nВами созданные ингредиенты:\n\n");
        if (!noReq.empty()) {
            text += "⚪️ Без запроса на публикацию:\n";
            for (const auto& ing : noReq) {
                text += std::format("• {}\n", ing.name);
            }
        }
        if (!pending.empty()) {
            text += "\n🟡 На рассмотрении:\n";
            for (const auto& ing : pending) {
                text += std::format("• {}\n", ing.name);
            }
        }
        if (!rejected.empty()) {
            text += "\n🔴 Отклонены:\n";
            for (const auto& ing : rejected) {
                text += std::format("• {}\n", ing.name);
            }
        }
        if (!accepted.empty()) {
            text += "\n🟢 Приняты:\n";
            for (const auto& ing : accepted) {
                text += std::format("• {}\n", ing.name);
            }
        }
        keyboard[0].push_back(makeCallbackButton(u8"🆕 Создать", "create"));
        keyboard[1].push_back(makeCallbackButton(u8"🚮 Удалить", "delete"));
        keyboard[2].push_back(makeCallbackButton(u8"📢 Опубликовать", "publish"));
        keyboard[3].push_back(makeCallbackButton(u8"↩️ Назад", "back"));
    }
    return std::make_pair(text, keyboard);
}

} // namespace

void renderCustomIngredientsList(
    bool toBeEdited, UserId userId, ChatId chatId, BotRef bot, api::IngredientsApiRef api) {
    const std::size_t numOfIngredientsOnPage = 500;

    auto ingredientsList = api.customIngredientsSearch(userId, "", 0, numOfIngredientsOnPage);

    auto res = constructMessage(ingredientsList);
    auto text = res.first;
    auto keyboard = res.second;
    if (toBeEdited) {
        if (auto messageId = message::getMessageId(userId))
            bot.editMessageText(text, chatId, *messageId, makeKeyboardMarkup(std::move(keyboard)), "MarkdownV2");

    } else {
        auto message = bot.sendMessage(chatId, text, makeKeyboardMarkup(std::move(keyboard)), "MarkdownV2");
        message::addMessageId(userId, message->messageId);
    }
}

} // namespace cookcookhnya::render::personal_account::ingredients
