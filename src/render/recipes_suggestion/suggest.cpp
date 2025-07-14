#include "suggest.hpp"

#include "backend/id_types.hpp"
#include "backend/models/recipe.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"

#include "utils/utils.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <format>
#include <string>
#include <vector>

namespace cookcookhnya::render::recipes_suggestion {

// offset is variable which defines amout of rows before beggining of paging
// fullKeyBoardSize is self explanatory
InlineKeyboard constuctNavigationsMarkup(size_t offset,
                                         size_t fullKeyBoardSize,
                                         size_t pageNo,
                                         size_t numOfRecipesOnPage,
                                         api::models::recipe::RecipesList recipesList) {
    const int amountOfRecipes = recipesList.recipesFound;
    int maxPageNum =
        static_cast<int>(std::ceil(static_cast<double>(amountOfRecipes) / static_cast<double>(numOfRecipesOnPage)));

    const size_t recipesToShow = std::min(numOfRecipesOnPage, recipesList.recipesPage.size());
    const bool ifMaxPage = amountOfRecipes - (static_cast<int>(numOfRecipesOnPage) * (static_cast<int>(pageNo) + 1)) <=
                           0; // + 1 because of the 0-indexing, as comparisson is between num of recipes gotten and that
                              // will be actually shown
    if (offset + recipesToShow > fullKeyBoardSize) { // IN ERROR HANDLING MAY USE ASSERT
        InlineKeyboard error(0);
        return error;
    }
    const size_t arrowsRow = offset + recipesToShow;

    InlineKeyboard keyboard(fullKeyBoardSize);
    int counter = 0;
    for (std::size_t i = 0; i < recipesToShow; i++) {
        // Print on button in form "1. {Recipe}"
        keyboard[i + offset].push_back(
            makeCallbackButton(std::format("{}. {} [{} из {}]",
                                           1 + counter + ((pageNo)*numOfRecipesOnPage),
                                           recipesList.recipesPage[counter].name,
                                           recipesList.recipesPage[counter].available,
                                           recipesList.recipesPage[counter].total),
                               std::format("recipe: {}", recipesList.recipesPage[counter].id))); // RECIPE ID
        counter++;
    }
    keyboard[arrowsRow].reserve(3);

    // Helps to reduce code. Power of C++ YEAH BABE!
    uint8_t b = 0;

    // Simply enamurate every case
    if (pageNo == 0) {
        if (!ifMaxPage) {
            b |= uint8_t{0x1};
        }
    } else if (ifMaxPage) {
        b |= uint8_t{0x2};
    } else {
        b |= uint8_t{0x3};
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
            keyboard[arrowsRow].push_back(makeCallbackButton(u8"ㅤ", "dontHandle"));
        }
    }
    // Put pageNo as button
    keyboard[arrowsRow].insert(keyboard[arrowsRow].begin() + 1,
                               makeCallbackButton(std::format("{} из {}", pageNo + 1, maxPageNum), "dontHandle"));
    return keyboard;
}

InlineKeyboard
constructMarkup(size_t pageNo, size_t numOfRecipesOnPage, api::models::recipe::RecipesList& recipesList) {

    const size_t numOfRows = 2; // 1 for back button return, 1 for arrows (ALWAYS ACCOUNT ARROWS)
    const size_t offset = 0;    // Number of rows before list
    const size_t recipesToShow = std::min(numOfRecipesOnPage, recipesList.recipesPage.size());

    const size_t arrowsRow = offset + recipesToShow; // 1 because of the offset of add/delete row

    InlineKeyboard keyboard =
        constuctNavigationsMarkup(offset, numOfRows + recipesToShow, pageNo, numOfRecipesOnPage, recipesList);
    if (keyboard.empty()) { // If error happened ADD PROPER ERROR HANDLING IF FUNCTION WILL BE REUSED
        return keyboard;
    }

    keyboard[arrowsRow + 1].push_back(
        makeCallbackButton(u8"↩️ Назад",
                           "backFromCustomRecipes")); // To LAST row add "backFromCustomRecipes"
    return keyboard;
}

void editRecipesSuggestion(const std::vector<api::StorageId>& storageIds,
                           size_t pageNo,
                           UserId userId,
                           ChatId chatId,
                           BotRef bot,
                           RecipesApiRef recipesApi) {
    const std::string pageInfo = utils::utf8str(u8"🔪 Рецепты подобранные специально для вас");

    auto messageId = message::getMessageId(userId);

    const size_t numOfRecipesOnPage = 5;

    auto recipesList = recipesApi.getRecipeList(userId,
                                                numOfRecipesOnPage,
                                                static_cast<size_t>((pageNo)*numOfRecipesOnPage),
                                                storageIds); // Take storages of user from backend

    if (messageId) {
        bot.editMessageText(pageInfo,
                            chatId,
                            *messageId,
                            "",
                            "",
                            nullptr,
                            makeKeyboardMarkup(constructMarkup(pageNo, numOfRecipesOnPage, recipesList)));
    }
}

} // namespace cookcookhnya::render::recipes_suggestion
