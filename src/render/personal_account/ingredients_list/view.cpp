#include "view.hpp"

#include "backend/models/ingredient.hpp"
#include "backend/models/publication_request_status.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils/to_string.hpp"
#include "utils/utils.hpp"

#include <cstddef>
#include <format>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace cookcookhnya::render::personal_account::ingredients {

using namespace tg_types;

namespace {

std::vector<TgBot::InlineKeyboardButton::Ptr> constructNavigationButtons(std::size_t pageNo, std::size_t maxPageNum) {
    std::vector<TgBot::InlineKeyboardButton::Ptr> buttons;
    auto forward = makeCallbackButton(u8"▶️", "next");
    auto backward = makeCallbackButton(u8"◀️", "prev");
    auto dont_handle = makeCallbackButton(u8"ㅤ", "dont_handle");
    auto page = makeCallbackButton(std::format("{} из {}", (pageNo + 1), maxPageNum), "dont_handle");
    if (pageNo == maxPageNum) {
        buttons.push_back(backward);
        buttons.push_back(page);
        buttons.push_back(dont_handle);
    } else if (pageNo == 0) {
        buttons.push_back(dont_handle);
        buttons.push_back(page);
        buttons.push_back(forward);
    } else {
        buttons.push_back(backward);
        buttons.push_back(page);
        buttons.push_back(forward);
    }
    return buttons;
}

std::pair<std::string, InlineKeyboard> constructMessage( // NOLINT(*complexity*)
    size_t pageNo,
    size_t numOfIngredientsOnPage,
    api::models::ingredient::CustomIngredientList& ingredientsList) {
    std::size_t numOfRows = 0;
    if (ingredientsList.found == 0)
        numOfRows = 2;
    else if (ingredientsList.found <= numOfIngredientsOnPage)
        numOfRows = 3;
    else
        numOfRows = 4;
    std::string text;
    InlineKeyboard keyboard(numOfRows);

    if (ingredientsList.found == 0) {
        text =
            utils::utf8str(u8"📋 Вы находитесь в Мои ингредиенты\\. Создавайте и делитесь новыми ингредиентами\\.\n\n");
        keyboard[0].push_back(makeCallbackButton(u8"🆕 Создать", "create"));
        keyboard[1].push_back(makeCallbackButton(u8"↩️ Назад", "back"));
    } else if (ingredientsList.found <= numOfIngredientsOnPage && pageNo == 0) {
        text = utils::utf8str(u8"📋 Вы находитесь в Мои ингредиенты\\. \nВами созданные ингредиенты:\n\n");
        for (const auto& ing : ingredientsList.page) {
            if (ing.moderationStatus == api::models::moderation::PublicationRequestStatus::NO_REQUEST) {
                text += std::format("• {}\n", ing.name);
            } else {
                if (ing.reason) {
                    text += std::format(
                    "• {}, Статус: {}, Причина: {}\n", ing.name, utils::to_string(ing.moderationStatus), *ing.reason);
                } else {
                    text += std::format("• {}, Статус: {}\n", ing.name, utils::to_string(ing.moderationStatus));
                }
            }
        }
        keyboard[0].push_back(makeCallbackButton(u8"🆕 Создать", "create"));
        keyboard[1].push_back(makeCallbackButton(u8"📢 Опубликовать", "publish"));
        keyboard[2].push_back(makeCallbackButton(u8"↩️ Назад", "back"));
    } else {
        text = utils::utf8str(u8"📋 Вы находитесь в Мои ингредиенты\\. \nВами созданные ингредиенты:\n\n");
        for (const auto& ing : ingredientsList.page) {
            if (ing.moderationStatus == api::models::moderation::PublicationRequestStatus::NO_REQUEST) {
                text += std::format("• {}\n", ing.name);
            } else {
                if (ing.reason) {
                    text += std::format(
                    "• {}, Статус: {}, Причина: {}\n", ing.name, utils::to_string(ing.moderationStatus), *ing.reason);
                } else {
                    text += std::format("• {}, Статус: {}\n", ing.name, utils::to_string(ing.moderationStatus));
                }
            }

        }
        keyboard[0].push_back(makeCallbackButton(u8"🆕 Создать", "create"));
        keyboard[1].push_back(makeCallbackButton(u8"📢 Опубликовать", "publish"));
        keyboard[2].reserve(3);
        const std::size_t maxPageNum =
            std::ceil(static_cast<double>(ingredientsList.found) / static_cast<double>(numOfIngredientsOnPage));
        for (const auto& navigButton : constructNavigationButtons(pageNo, maxPageNum)) {
            keyboard[2].push_back(navigButton);
        }
        keyboard[3].push_back(makeCallbackButton(u8"↩️ Назад", "back"));
    }
    return std::make_pair(text, keyboard);
}

} // namespace

void renderCustomIngredientsList(
    bool toBeEdited, std::size_t pageNo, UserId userId, ChatId chatId, BotRef bot, IngredientsApiRef api) {
    const std::size_t numOfIngredientsOnPage = 5;

    auto ingredientsList =
        api.customIngredientsSearch(userId, "", 0, numOfIngredientsOnPage, pageNo * numOfIngredientsOnPage);

    auto res = constructMessage(pageNo, numOfIngredientsOnPage, ingredientsList);
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
