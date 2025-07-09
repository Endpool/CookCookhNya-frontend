#include "render/custom_recipes_list/view.hpp"
#include "backend/models/recipe.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include <cstddef>
#include <string>
#include <vector>

namespace cookcookhnya::render::custom_recipes_list {
// offset is variable which defines amout of rows before beggining of paging
// fullKeyBoardSize is self explanatory
InlineKeyboard constuctNavigationsMarkup(size_t offset,
                                         size_t fullKeyBoardSize,
                                         int pageNo,
                                         int numOfRecipesOnPage,
                                         api::models::recipe::CustomRecipesList recipesList) {
    const int amountOfRecipes = recipesList.recipesFound;
    const bool ifMaxPage = amountOfRecipes - (numOfRecipesOnPage * (pageNo + 1)) <=
                           0; // + 1 because of the 0-indexing, as comparisson is between num of recipes gotten and that
                              // will be actually shown

    if (offset + numOfRecipesOnPage > fullKeyBoardSize) {
        InlineKeyboard error(0);
        return error;
    }
    const size_t arrowsRow = offset + numOfRecipesOnPage;

    InlineKeyboard keyboard(fullKeyBoardSize);
    int counter = 0;
    for (std::size_t i = offset; i < numOfRecipesOnPage + offset; i++) {
        // Print on button in form "1. {Recipe}"
        keyboard[i].push_back(detail::makeCallbackButton(
            std::format("{}. {}", 1 + counter + ((pageNo)*numOfRecipesOnPage), recipesList.recipesPage[counter].name),
            std::format("recipe: {}", recipesList.recipesPage[counter].id))); // RECIPE ID
        counter++;
    }

    // ONLY ONE CASE: WHEN First page has all recipes already
    if (pageNo == 0) {
        if (ifMaxPage) {
            // if didn't worked then the amount of recipes is less then i want to display -> no need for next or prev
            // button
            keyboard[arrowsRow].reserve(3);
            keyboard[arrowsRow].push_back(detail::makeCallbackButton(u8"ㅤ", " "));
            keyboard[arrowsRow].push_back(
                detail::makeCallbackButton(std::to_string(pageNo + 1), " ")); // Put pageNo as button
            keyboard[arrowsRow].push_back(detail::makeCallbackButton(u8"ㅤ", " "));
            return keyboard;
        }

        // If first page wasn't didn't represent all recipes in one page then the field with arrows is required
        // 1 for back button return, 1 for two arrows next and prev, and other buttons are recipes

        // If pageNo == 0 and it's not 1st page then show only next button
        keyboard[arrowsRow].reserve(3);
        keyboard[arrowsRow].push_back(detail::makeCallbackButton(u8"ㅤ", " "));
        keyboard[arrowsRow].push_back(
            detail::makeCallbackButton(std::to_string(pageNo + 1), " ")); // Put pageNo as button
        keyboard[arrowsRow].push_back(detail::makeCallbackButton(u8"⏭️", std::to_string(pageNo + 1)));

        return keyboard;
    }

    // If first page wasn't didn't represent all recipes in one page then the field with arrows is required
    if (!ifMaxPage) {
        // Show both possible ways
        keyboard[arrowsRow].reserve(3);
        keyboard[arrowsRow].push_back(detail::makeCallbackButton(u8"⏮️", std::to_string(pageNo - 1)));
        keyboard[arrowsRow].push_back(
            detail::makeCallbackButton(std::to_string(pageNo + 1), " ")); // Put pageNo as button
        keyboard[arrowsRow].push_back(detail::makeCallbackButton(u8"⏭️", std::to_string(pageNo + 1)));
    } else {
        // If pageNo == maxPage then it's last page -> won't show button next
        keyboard[arrowsRow].reserve(3);
        keyboard[arrowsRow].push_back(detail::makeCallbackButton(u8"⏮️", std::to_string(pageNo - 1)));
        keyboard[arrowsRow].push_back(
            detail::makeCallbackButton(std::to_string(pageNo + 1), " ")); // Put pageNo as button
        keyboard[arrowsRow].push_back(detail::makeCallbackButton(u8"ㅤ", " "));
    }

    return keyboard;
}

InlineKeyboard
constructMarkup(int pageNo, int numOfRecipesOnPage, api::models::recipe::CustomRecipesList& recipesList) {

    const size_t numOfRows = 3;                           // 1 for back button return, 1 for arrows, 1
                                                          // for adding new recipe - other buttons are recipes
    size_t offset = 1;                                    // Number of rows before list
    const size_t arrowsRow = offset + numOfRecipesOnPage; // 1 because of the offset of add/delete row

    InlineKeyboard keyboard =
        constuctNavigationsMarkup(offset, numOfRows + numOfRecipesOnPage, pageNo, numOfRecipesOnPage, recipesList);
    if (keyboard.empty()) { // If error happened
        return keyboard;
    }
    keyboard[0].push_back(detail::makeCallbackButton(u8"Создать", "custom_recipe_create"));

    keyboard[arrowsRow + 1].push_back(
        detail::makeCallbackButton(u8"↩️ Назад",
                                   "backFromCustomRecipes")); // To LAST row add "backFromCustomRecipes"
    return keyboard;
}

void renderCustomRecipesList(int pageNo, UserId userId, ChatId chatId, BotRef bot, RecipesApiRef recipesApi) {
    const std::string pageInfo = utils::utf8str(u8"🔪 Рецепты созданные вами");

    auto messageId = message::getMessageId(userId);

    const int numOfRecipesOnPage = 5;

    /*auto recipesList =
        recipesApi.getPrivateRecipeList(userId,
                                        numOfRecipesOnPage,
                                        (pageNo)*numOfRecipesOnPage); //*/
    // api::models::recipe::CustomRecipeSummary recipeExample1 = {.id = 1, .name = "asd"};

    std::vector<api::models::recipe::CustomRecipeSummary> recipesExample = {{.id = 1, .name = "asd"},
                                                                            {.id = 2, .name = "asasdd"},
                                                                            {.id = 2, .name = "asasdd"},
                                                                            {.id = 2, .name = "asasdd"},
                                                                            {.id = 2, .name = "asasdd"},
                                                                            {.id = 2, .name = "asasdd"},
                                                                            {.id = 2, .name = "asasdd"},
                                                                            {.id = 2, .name = "asasdd"},
                                                                            {.id = 2, .name = "asasdd"}};
    api::models::recipe::CustomRecipesList recipesList{.recipesPage = recipesExample, .recipesFound = 4};
    if (messageId) {
        bot.editMessageText(pageInfo,
                            chatId,
                            *messageId,
                            "",
                            "",
                            nullptr,
                            detail::makeKeyboardMarkup(constructMarkup(pageNo, numOfRecipesOnPage, recipesList)));
    }
}
} // namespace cookcookhnya::render::custom_recipes_list
