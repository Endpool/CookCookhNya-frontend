#include "view.hpp"

#include "backend/models/recipe.hpp"
#include "backend/models/storage.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils/to_string.hpp"
#include "utils/utils.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <format>
#include <string>
#include <vector>

namespace cookcookhnya::render::recipes_suggestions {

// offset is variable which defines amout of rows before beggining of paging
// fullKeyBoardSize is self explanatory
InlineKeyboard constructNavigationsMarkup(std::size_t offset,
                                          std::size_t fullKeyBoardSize,
                                          std::size_t pageNo,
                                          std::size_t numOfRecipesOnPage,
                                          api::models::recipe::RecipesList recipesList) {
    const std::size_t amountOfRecipes = recipesList.found;
    std::size_t maxPageNum = std::ceil(static_cast<double>(amountOfRecipes) / static_cast<double>(numOfRecipesOnPage));

    const size_t recipesToShow = std::min(numOfRecipesOnPage, recipesList.page.size());
    // + 1 because of the 0-indexing, as comparisson is between num of recipes gotten and that
    // will be actually shown
    const bool ifMaxPage =
        static_cast<int>(amountOfRecipes) - static_cast<int>(numOfRecipesOnPage) * (static_cast<int>(pageNo) + 1) <= 0;

    if (offset + recipesToShow > fullKeyBoardSize) { // IN ERROR HANDLING MAY USE ASSERT
        InlineKeyboard error(0);
        return error;
    }
    const size_t arrowsRow = offset + recipesToShow;

    InlineKeyboard keyboard(pageNo == 0 && ifMaxPage ? fullKeyBoardSize - 1 : fullKeyBoardSize);
    int counter = 0;
    for (std::size_t i = 0; i < recipesToShow; i++) {
        keyboard[i + offset].push_back(makeCallbackButton(std::format("{}. {} [{} из {}]",
                                                                      1 + counter + ((pageNo)*numOfRecipesOnPage),
                                                                      recipesList.page[counter].name,
                                                                      recipesList.page[counter].available,
                                                                      recipesList.page[counter].total),
                                                          std::format("r{}", recipesList.page[counter].id)));
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
            keyboard[arrowsRow].push_back(makeCallbackButton(u8"ㅤ", "dont_handle"));
        }
    }
    // Put pageNo as button
    keyboard[arrowsRow].insert(keyboard[arrowsRow].begin() + 1,
                               makeCallbackButton(std::format("{} из {}", pageNo + 1, maxPageNum), "dont_handle"));
    keyboard[arrowsRow + 1].push_back(makeCallbackButton(u8"↩️ Назад", "back"));
    return keyboard;
}

InlineKeyboard constructOnlyBack() {
    InlineKeyboard keyboard(1);
    keyboard[0].push_back(makeCallbackButton(u8"↩️ Назад", "back"));
    return keyboard;
}

InlineKeyboard
constructMarkup(size_t pageNo, size_t numOfRecipesOnPage, api::models::recipe::RecipesList& recipesList) {

    const size_t numOfRows = 2; // 1 for back button return, 1 for arrows (ALWAYS ACCOUNT ARROWS)
    const size_t offset = 0;    // Number of rows before list
    const size_t recipesToShow = std::min(numOfRecipesOnPage, recipesList.page.size());

    InlineKeyboard keyboard =
        recipesList.found == 0
            ? constructOnlyBack()
            : constructNavigationsMarkup(offset, numOfRows + recipesToShow, pageNo, numOfRecipesOnPage, recipesList);
    if (keyboard.empty()) { // If error happened ADD PROPER ERROR HANDLING IF FUNCTION WILL BE REUSED
        return keyboard;
    }

    return keyboard;
}

void renderRecipesSuggestion(std::vector<api::models::storage::StorageSummary>& storages,
                             size_t pageNo,
                             UserId userId,
                             ChatId chatId,
                             BotRef bot,
                             RecipesApiRef recipesApi) {
    const std::string pageInfo = utils::utf8str(u8"🔪 Рецепты подобранные специально для вас");

    auto messageId = message::getMessageId(userId);

    const size_t numOfRecipesOnPage = 5;
    const size_t numOfRecipes = 500;

    auto recipesList = recipesApi.getSuggestedRecipesList(userId, storages, numOfRecipes, pageNo * numOfRecipesOnPage);

    if (messageId) {
        bot.editMessageText(
            pageInfo, chatId, *messageId, makeKeyboardMarkup(constructMarkup(pageNo, numOfRecipesOnPage, recipesList)));
    }
}

} // namespace cookcookhnya::render::recipes_suggestions
