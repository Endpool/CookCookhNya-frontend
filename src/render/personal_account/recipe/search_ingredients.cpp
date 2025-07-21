#include "search_ingredients.hpp"

#include "backend/models/ingredient.hpp"
#include "message_tracker.hpp"
#include "patched_bot.hpp"
#include "render/common.hpp"
#include "render/pagination.hpp"
#include "states.hpp"
#include "utils/utils.hpp"

#include <tgbot/types/InlineKeyboardButton.h>

#include <cstddef>
#include <format>
#include <memory>
#include <ranges>
#include <string>
#include <utility>

namespace TgBot {
class InlineKeyboardMarkup;
} // namespace TgBot

namespace cookcookhnya::render::personal_account::recipe {

using namespace api::models::ingredient;
using namespace tg_types;
using namespace std::views;
using std::ranges::to;

namespace {

std::shared_ptr<TgBot::InlineKeyboardMarkup>
constructKeyboard(std::size_t pageNo, std::size_t pageSize, const states::CustomRecipeIngredientsSearch& state) {
    InlineKeyboardBuilder keyboard;

    auto searchButton = std::make_shared<TgBot::InlineKeyboardButton>();
    searchButton->text = utils::utf8str(u8"✏️ Редактировать");
    searchButton->switchInlineQueryCurrentChat = "";
    keyboard << std::move(searchButton) << NewRow{};

    auto makeRecipeButton = [](const IngredientSearchForRecipeItem& ing) {
        return makeCallbackButton((ing.isInRecipe ? "[ + ] " : "[ㅤ] ") + ing.name, utils::to_string(ing.id));
    };
    keyboard << constructPagination(pageNo, pageSize, state.totalFound, state.searchItems, makeRecipeButton)
             << makeCallbackButton(u8"↩️ Назад", "back");

    return keyboard;
}

} // namespace

void renderRecipeIngredientsSearch(const states::CustomRecipeIngredientsSearch& state,
                                   std::size_t numOfIngredientsOnPage,
                                   UserId userId,
                                   ChatId chatId,
                                   BotRef bot) {
    std::string list = state.recipeIngredients.getValues() |
                       transform([](auto& i) { return std::format("• {}\n", i.name); }) | join | to<std::string>();
    auto text = utils::utf8str(u8"📝Нажмите на кнопку ✏️ Редактировать и начните вводить названия продуктов:\n\n") +
                std::move(list);

    if (auto messageId = message::getMessageId(userId))
        bot.editMessageText(text, chatId, *messageId, constructKeyboard(state.pageNo, numOfIngredientsOnPage, state));
}

void renderSuggestIngredientCustomisation(const states::CustomRecipeIngredientsSearch& state,
                                          UserId userId,
                                          ChatId chatId,
                                          BotRef bot) {
    InlineKeyboard keyboard(3);
    std::string text = utils::utf8str(u8"📝 Продолжите редактирование запроса или объявите личный ингредиент");

    auto searchButton = std::make_shared<TgBot::InlineKeyboardButton>();
    searchButton->text = utils::utf8str(u8"✏️ Редактировать");
    searchButton->switchInlineQueryCurrentChat = "";
    keyboard[0].push_back(std::move(searchButton));
    // Mark as ingredient
    keyboard[1].push_back(
        makeCallbackButton(std::format("Создать личный ингредиент: {}", state.query), "i" + state.query));
    keyboard[2].push_back(makeCallbackButton(u8"↩️ Назад", "back"));

    if (auto messageId = message::getMessageId(userId))
        bot.editMessageText(text, chatId, *messageId, makeKeyboardMarkup(std::move(keyboard)));
}

} // namespace cookcookhnya::render::personal_account::recipe
