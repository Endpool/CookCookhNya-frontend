#include "view.hpp"

#include "backend/models/recipe.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "render/personal_account/recipes_list/view.hpp"
#include "utils.hpp"

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
                                          api::models::recipe::CustomRecipesList recipesList) {
    const int amountOfRecipes = recipesList.recipesFound;
    int maxPageNum =
        static_cast<int>(std::ceil(static_cast<double>(amountOfRecipes) / static_cast<double>(numOfRecipesOnPage)));

    const size_t recipesToShow = std::min(numOfRecipesOnPage, recipesList.recipesPage.size());

    const bool ifMaxPage = amountOfRecipes - (static_cast<int>(numOfRecipesOnPage) * (static_cast<int>(pageNo) + 1)) <=
                           0; // + 1 because of the 0-indexing, as comparisson is between num of recipes gotten and that
                              // will be actually shown

    if (offset + recipesToShow >= fullKeyBoardSize) {
        InlineKeyboard error(0);
        return error;
    }
    const size_t arrowsRow = offset + recipesToShow;

    InlineKeyboard keyboard(fullKeyBoardSize);
    int counter = 0;
    for (std::size_t i = 0; i < recipesToShow; i++) {
        // Print on button in form "1. {Recipe}"
        keyboard[i + offset].push_back(makeCallbackButton(
            std::format("{}. {}", 1 + counter + ((pageNo)*numOfRecipesOnPage), recipesList.recipesPage[counter].name),
            std::format("recipe: {}", recipesList.recipesPage[counter].id))); // RECIPE ID
        counter++;
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
                keyboard[arrowsRow].push_back(makeCallbackButton(u8"◀️", std::to_string(pageNo - 1))); // left
            } else {
                keyboard[arrowsRow].push_back(makeCallbackButton(u8"▶️", std::to_string(pageNo + 1))); // right
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
constructMarkup(size_t pageNo, size_t numOfRecipesOnPage, api::models::recipe::CustomRecipesList& recipesList) {
    // 1 for back button return, 1 for arrows (ALWAYS ACCOUNT ARROWS), 1
    // for adding new recipe - other buttons are recipes
    const size_t numOfRows = 3;
    const size_t offset = 1; // Number of rows before list

    const size_t recipesToShow = std::min(numOfRecipesOnPage, recipesList.recipesPage.size());

    const size_t arrowsRow = offset + recipesToShow; // 1 because of the offset of add/delete row

    InlineKeyboard keyboard =
        constructNavigationsMarkup(offset, numOfRows + recipesToShow, pageNo, numOfRecipesOnPage, recipesList);
    if (keyboard.empty()) { // If error happened
        return keyboard;
    }
    keyboard[0].push_back(makeCallbackButton(u8"Создать", "custom_recipe_create"));

    keyboard[arrowsRow + 1].push_back(
        makeCallbackButton(u8"↩️ Назад",
                           "backFromCustomRecipes")); // To LAST row add "backFromCustomRecipes"
    return keyboard;
}

void renderCustomRecipesList(size_t pageNo, UserId userId, ChatId chatId, BotRef bot, RecipesApiRef /*recipesApi*/) {
    const std::string pageInfo = utils::utf8str(u8"🔪 Рецепты созданные вами");

    auto messageId = message::getMessageId(userId);

    const int numOfRecipesOnPage = 5;

    // UNCOMMENT WHEN BACKEND IS READY
    /*auto recipesList =
        recipesApi.getPrivateRecipeList(userId,
                                        numOfRecipesOnPage,
                                        (pageNo)*numOfRecipesOnPage); //*/
    // api::models::recipe::CustomRecipeSummary recipeExample1 = {.id = 1, .name = "asd"};

    const std::vector<api::models::recipe::CustomRecipeSummary> recipesExample = {{.id = 1, .name = "asd"},
                                                                                  {.id = 2, .name = "asasdd"},
                                                                                  {.id = 2, .name = "asasdd"},
                                                                                  {.id = 2, .name = "asasdd"},
                                                                                  {.id = 2, .name = "asasdd"},
                                                                                  {.id = 2, .name = "asasdd"},
                                                                                  {.id = 2, .name = "asasdd"},
                                                                                  {.id = 2, .name = "asasdd"},
                                                                                  {.id = 2, .name = "asasdd"}};
    api::models::recipe::CustomRecipesList recipesList{.recipesPage = recipesExample,
                                                       .recipesFound = static_cast<int>(recipesExample.size())};
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
} // namespace cookcookhnya::render::personal_account::recipes
