#include "view.hpp"

#include "backend/api/publicity_filter.hpp"
#include "backend/models/ingredient.hpp"
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

std::pair<std::string, std::vector<TgBot::InlineKeyboardButton::Ptr>> constructNavigationMessage(
    std::size_t pageNo, std::size_t numOfRecipesOnPage, api::models::ingredient::IngredientList& ingredientsList) {
    const size_t amountOfRecipes = ingredientsList.found;
    const std::size_t maxPageNum =
        std::ceil(static_cast<double>(amountOfRecipes) / static_cast<double>(numOfRecipesOnPage));

    std::string text;

    text = utils::utf8str(u8"📋 Вы находитесь в Мои ингредиенты\\. \nВами созданные ингредиенты:\n\n");
    for (auto& ing : ingredientsList.page) {
        if (ing.status) {
            text += std::format("• {}, Статус: {}\n", utils::to_string(ing.status.value()), ing.name);
        }
    }

    std::vector<TgBot::InlineKeyboardButton::Ptr> buttons;
    if (pageNo == maxPageNum) {
        buttons.push_back(makeCallbackButton(u8"◀️", utils::to_string(pageNo - 1)));
        buttons.push_back(makeCallbackButton(std::format("{} из {}", (pageNo + 1), (maxPageNum + 1)), "dont_handle"));
        buttons.push_back(makeCallbackButton(u8"ㅤ", "dont_handle"));
    } else if (pageNo == 0) {
        buttons.push_back(makeCallbackButton(u8"ㅤ", "dont_handle"));
        buttons.push_back(makeCallbackButton(std::format("{} из {}", (pageNo + 1), (maxPageNum + 1)), "dont_handle"));
        buttons.push_back(makeCallbackButton(u8"▶️", utils::to_string(pageNo + 1)));
    } else {
        buttons.push_back(makeCallbackButton(u8"◀️", utils::to_string(pageNo - 1)));
        buttons.push_back(makeCallbackButton(std::format("{} из {}", (pageNo + 1), (maxPageNum + 1)), "dont_handle"));
        buttons.push_back(makeCallbackButton(u8"▶️", utils::to_string(pageNo + 1)));
    }
    return std::make_pair(text, buttons);
}

std::pair<std::string, InlineKeyboard> constructMessage(size_t pageNo,
                                                        size_t numOfIngredientsOnPage,
                                                        api::models::ingredient::IngredientList& ingredientsList) {
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
    } else if (ingredientsList.found <= numOfIngredientsOnPage) {
        text = utils::utf8str(u8"📋 Вы находитесь в Мои ингредиенты\\. \nВами созданные ингредиенты:\n\n");
        for (const auto& ing : ingredientsList.page) {
            if (ing.status) {
                text += std::format("• {}, Статус: {}\n", utils::to_string(*ing.status), ing.name);
            }
        }
        keyboard[0].push_back(makeCallbackButton(u8"🆕 Создать", "create"));
        keyboard[1].push_back(makeCallbackButton(u8"📢 Опубликовать", "publish"));
        keyboard[2].push_back(makeCallbackButton(u8"↩️ Назад", "back"));
    } else {
        auto message = constructNavigationMessage(pageNo, numOfIngredientsOnPage, ingredientsList);
        text = message.first;
        keyboard[0].push_back(makeCallbackButton(u8"🆕 Создать", "create"));
        keyboard[1].push_back(makeCallbackButton(u8"📢 Опубликовать", "publish"));
        keyboard[2].reserve(3);
        for (const auto& navigButton : message.second) {
            keyboard[2].push_back(navigButton);
        }
        keyboard[3].push_back(makeCallbackButton(u8"↩️ Назад", "back"));
    }
    return std::make_pair(text, keyboard);
}

} // namespace

void renderCustomIngredientsList(
    bool toBeEdited, std::size_t pageNo, UserId userId, ChatId chatId, BotRef bot, IngredientsApiRef api) {
    const std::size_t numOfIngredientsOnPage = 10;

    auto ingredientsList =
        api.getList(userId, PublicityFilterType::Custom, numOfIngredientsOnPage, pageNo * numOfIngredientsOnPage);

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
