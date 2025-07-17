#include "search_ingredients.hpp"

#include "backend/models/ingredient.hpp"
#include "message_tracker.hpp"
#include "patched_bot.hpp"
#include "render/common.hpp"
#include "states.hpp"
#include "utils/to_string.hpp"
#include "utils/utils.hpp"

#include <tgbot/types/InlineKeyboardButton.h>

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <format>
#include <memory>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

namespace cookcookhnya::render::recipe::ingredients {

using namespace api::models::ingredient;
using namespace tg_types;

namespace {

InlineKeyboard constructNavigationsMarkup(size_t offset,
                                          size_t fullKeyBoardSize,
                                          size_t numOfRecipesOnPage,
                                          const states::CustomRecipeIngredientsSearch& state) {
    using namespace std::views;
    const size_t amountOfRecipes = state.totalFound;
    int maxPageNum =
        static_cast<int>(std::ceil(static_cast<double>(amountOfRecipes) / static_cast<double>(numOfRecipesOnPage)));

    const size_t recipesToShow = std::min(numOfRecipesOnPage, state.searchItems.size());
    // + 1 because of the 0-indexing, as comparisson is between num of recipes gotten and that
    // will be actually shown
    const bool ifMaxPage = static_cast<int>(amountOfRecipes) -
                               static_cast<int>(numOfRecipesOnPage) * (static_cast<int>(state.pageNo) + 1) <=
                           0;

    if (offset + recipesToShow >= fullKeyBoardSize) {
        InlineKeyboard error(0);
        return error;
    }
    const size_t arrowsRow = offset + recipesToShow;
    // Don't reserve for arrows if it's first page is max(im)
    InlineKeyboard keyboard(state.pageNo == 0 && ifMaxPage ? fullKeyBoardSize - 1 : fullKeyBoardSize);

    auto searchButton = std::make_shared<TgBot::InlineKeyboardButton>();
    searchButton->text = utils::utf8str(u8"✏️ Редактировать");
    searchButton->switchInlineQueryCurrentChat = "";
    keyboard[0].push_back(std::move(searchButton));

    for (auto [row, ing] : zip(drop(keyboard, 1), state.searchItems))
        row.push_back(makeCallbackButton((ing.isInRecipe ? "[ + ] " : "[ㅤ] ") + ing.name, utils::to_string(ing.id)));

    if (state.pageNo == 0 && ifMaxPage) {
        // instead of arrows row
        keyboard[arrowsRow].push_back(makeCallbackButton(u8"↩️ Назад", "back"));
        return keyboard;
    }
    keyboard[arrowsRow].reserve(3);

    // Helps to reduce code. Power of C++ YEAH BABE!
    uint8_t b = 0;

    // Simply enamurate every case
    if (state.pageNo == 0) {
        if (!ifMaxPage) {
            b |= uint8_t{0b01};
        }
    } else if (ifMaxPage) {
        b |= uint8_t{0b10};
    } else {
        b |= uint8_t{0b11};
    }

    // Check from left to right due to buttons being displayed like that
    for (int i = 1; i >= 0; i--) {
        // Compare two bits under b mask. If 1 was on b mask then we need to place arrow somewhere
        if ((b & static_cast<uint8_t>((uint8_t{0b1} << static_cast<uint8_t>(i)))) ==
            (uint8_t{0b1} << static_cast<uint8_t>(i))) {
            // if we need to place arrow then check the i, which represents bit which we are checking right now
            if (i == 1) {
                keyboard[arrowsRow].push_back(makeCallbackButton(u8"◀️", "prev")); // left
            } else {
                keyboard[arrowsRow].push_back(makeCallbackButton(u8"▶️", "next")); // right
            }
        } else {
            keyboard[arrowsRow].push_back(makeCallbackButton(u8"ㅤ", "dont_handle"));
        }
    }
    // Put state.pageNo as button
    keyboard[arrowsRow].insert(
        keyboard[arrowsRow].begin() + 1,
        makeCallbackButton(std::format("{} из {}", state.pageNo + 1, maxPageNum), "dont_handle"));
    keyboard[arrowsRow + 1].push_back(makeCallbackButton(u8"↩️ Назад", "back"));
    return keyboard;
}

InlineKeyboard constructMarkup(size_t numOfRecipesOnPage, const states::CustomRecipeIngredientsSearch& state) {
    // 1 for back button return, 1 for arrows (ALWAYS ACCOUNT ARROWS), 1
    // for editing - other buttons are ingredients
    const size_t numOfRows = 3;
    const size_t offset = 1; // Number of rows before list

    const size_t recipesToShow = std::min(numOfRecipesOnPage, state.searchItems.size());

    InlineKeyboard keyboard = constructNavigationsMarkup(offset, numOfRows + recipesToShow, numOfRecipesOnPage, state);
    if (keyboard.empty()) { // If error happened
        return keyboard;
    }

    return keyboard;
}

} // namespace

void renderRecipeIngredientsSearch(const states::CustomRecipeIngredientsSearch& state,
                                   size_t numOfIngredientsOnPage,
                                   UserId userId,
                                   ChatId chatId,
                                   BotRef bot) {
    using namespace std::views;
    using std::ranges::to;

    std::string list = state.recipeIngredients.getAll() |
                       transform([](auto& i) { return std::format("• {}\n", i.name); }) | join | to<std::string>();

    auto text = utils::utf8str(u8"📝Нажмите на кнопку ✏️ Редактировать и начните вводить названия продуктов:\n\n") +
                std::move(list);
    if (auto messageId = message::getMessageId(userId)) {
        bot.editMessageText(text,
                            chatId,
                            *messageId,
                            "",
                            "",
                            nullptr,
                            makeKeyboardMarkup(constructMarkup(numOfIngredientsOnPage, state)));
    }
}

} // namespace cookcookhnya::render::recipe::ingredients
