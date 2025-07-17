#include "view.hpp"

#include "backend/api/common.hpp"
#include "backend/api/recipes.hpp"
#include "backend/models/recipe.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "render/personal_account/recipes_list/view.hpp"
#include "utils/to_string.hpp"
#include "utils/utils.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <format>
#include <string>
#include <vector>

namespace cookcookhnya::render::personal_account::recipes {

// offset is variable which defines amout of rows before beggining of paging
// fullKeyBoardSize is self explanatory
InlineKeyboard constructNavigationsMarkup(size_t offset,
                                          size_t fullKeyBoardSize,
                                          size_t pageNo,
                                          size_t numOfRecipesOnPage,
                                          api::models::recipe::RecipeSearchResponse& recipesList) {
    const size_t amountOfRecipes = recipesList.found;
    std::size_t maxPageNum = std::ceil(static_cast<double>(amountOfRecipes) / static_cast<double>(numOfRecipesOnPage));

    const size_t recipesToShow = std::min(numOfRecipesOnPage, recipesList.page.size());

    const bool ifMaxPage = amountOfRecipes - numOfRecipesOnPage * (pageNo + 1) <= 0;

    if (offset + recipesToShow >= fullKeyBoardSize) {
        InlineKeyboard error(0);
        return error;
    }
    const size_t arrowsRow = offset + recipesToShow;
    // Don't reserve for arrows if it's first page is max(im)
    InlineKeyboard keyboard(pageNo == 0 && ifMaxPage ? fullKeyBoardSize - 1 : fullKeyBoardSize);
    std::size_t counter = 0;
    for (std::size_t i = 0; i < recipesToShow; i++) {
        // Print on button in form "1. {Recipe}"
        keyboard[i + offset].push_back(makeCallbackButton(
            std::format("{}. {}", 1 + counter + ((pageNo)*numOfRecipesOnPage), recipesList.page[counter].name),
            std::format("r", recipesList.page[counter].id))); // RECIPE ID
        counter++;
    }
    if (pageNo == 0 && ifMaxPage) {
        // instead of arrows row
        keyboard[arrowsRow].push_back(makeCallbackButton(u8"↩️ Назад", "back"));
        return keyboard;
    }
    keyboard[arrowsRow].reserve(3);

    // Helps to reduce code. Power of C++ YEAH BABE!
    uint8_t b = 0;

    // Simply enamurate every case
    if (pageNo == 0) {
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
                keyboard[arrowsRow].push_back(makeCallbackButton(u8"◀️", utils::to_string(pageNo - 1))); // left
            } else {
                keyboard[arrowsRow].push_back(makeCallbackButton(u8"▶️", utils::to_string(pageNo + 1))); // right
            }
        } else {
            keyboard[arrowsRow].push_back(makeCallbackButton(u8"ㅤ", "dont_handle"));
        }
    }
    // Put pageNo as button
    keyboard[arrowsRow].insert(keyboard[arrowsRow].begin() + 1,
                               makeCallbackButton(std::format("{} из {}", pageNo + 1, maxPageNum), "dont_handle"));
    keyboard[arrowsRow + 1].push_back(makeCallbackButton(u8"↩️ Назад", "back"));
    return keyboard;
}

InlineKeyboard constructOnlyCreate() {
    InlineKeyboard keyboard(2);
    keyboard[1].push_back(makeCallbackButton(u8"↩️ Назад", "back"));
    return keyboard;
}

InlineKeyboard
constructMarkup(size_t pageNo, size_t numOfRecipesOnPage, api::models::recipe::RecipeSearchResponse& recipesList) {
    // 1 for back button return, 1 for arrows (ALWAYS ACCOUNT ARROWS), 1
    // for adding new recipe - other buttons are recipes
    const size_t numOfRows = 3;
    const size_t offset = 1; // Number of rows before list

    const size_t recipesToShow = std::min(numOfRecipesOnPage, recipesList.page.size());

    InlineKeyboard keyboard =
        recipesList.found == 0 ? constructOnlyCreate()
                               : constructNavigationsMarkup(
                                     offset, numOfRows + recipesToShow, (pageNo + 1), numOfRecipesOnPage, recipesList);
    if (keyboard.empty()) { // If error happened
        return keyboard;
    }
    keyboard[0].push_back(makeCallbackButton(u8"🆕 Создать", "custom_recipe_create"));

    return keyboard;
}

void renderCustomRecipesList(size_t pageNo, UserId userId, ChatId chatId, BotRef bot, RecipesApiRef recipesApi) {
    const std::string pageInfo = utils::utf8str(u8"🔪 Рецепты созданные вами");

    auto messageId = message::getMessageId(userId);

    const std::size_t numOfRecipesOnPage = 5;
    auto recipesList =
        recipesApi.getRecipesList(userId, "", 0, numOfRecipesOnPage, pageNo * numOfRecipesOnPage, filterType::Custom);
    if (messageId) {
        bot.editMessageText(
            pageInfo, chatId, *messageId, makeKeyboardMarkup(constructMarkup(pageNo, numOfRecipesOnPage, recipesList)));
    }
}
} // namespace cookcookhnya::render::personal_account::recipes
