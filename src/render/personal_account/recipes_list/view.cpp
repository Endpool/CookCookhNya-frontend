#include "view.hpp"

#include "backend/api/publicity_filter.hpp"
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

namespace {

// offset is variable which defines amout of rows before beggining of paging
// fullKeyBoardSize is self explanatory
InlineKeyboard constructNavigationsMarkup(std::size_t offset,
                                          std::size_t fullKeyBoardSize,
                                          std::size_t pageNo,
                                          std::size_t numOfRecipesOnPage,
                                          api::models::recipe::RecipesList& recipesList) {
    const size_t amountOfRecipes = recipesList.found;
    std::size_t maxPageNum = std::ceil(static_cast<double>(amountOfRecipes) / static_cast<double>(numOfRecipesOnPage));

    const size_t recipesToShow = std::min(numOfRecipesOnPage, recipesList.page.size());

    const bool lastPage = amountOfRecipes - numOfRecipesOnPage * (pageNo + 1) <= 0;

    if (offset + recipesToShow >= fullKeyBoardSize) {
        InlineKeyboard error(0);
        return error;
    }
    const size_t arrowsRow = offset + recipesToShow;
    // Don't reserve for arrows if it's first page is max(im)
    InlineKeyboard keyboard(pageNo == 0 && lastPage ? fullKeyBoardSize - 1 : fullKeyBoardSize);
    std::size_t counter = 0;
    for (std::size_t i = 0; i < recipesToShow; i++) {
        // Print on button in form "1. {Recipe}"
        keyboard[i + offset].push_back(makeCallbackButton(
            std::format("{}. {}", 1 + counter + ((pageNo)*numOfRecipesOnPage), recipesList.page[counter].name),
            std::format("r", recipesList.page[counter].id))); // RECIPE ID
        counter++;
    }
    if (pageNo == 0 && lastPage) {
        // instead of arrows row
        keyboard[arrowsRow].push_back(makeCallbackButton(u8"↩️ Назад", "back"));
        return keyboard;
    }
    keyboard[arrowsRow].reserve(3);

    enum PageArrows : std::uint8_t {
        NOTHING = 0b00U,
        LEFT = 0b01U,
        RIGHT = 0b10U,
    };

    PageArrows b = NOTHING;
    if (pageNo != 0)
        b = static_cast<PageArrows>(b | LEFT);
    if (!lastPage)
        b = static_cast<PageArrows>(b | RIGHT);

    if ((b & LEFT) != 0)
        keyboard[arrowsRow].push_back(makeCallbackButton(u8"◀️", utils::to_string(pageNo - 1))); // left
    else
        keyboard[arrowsRow].push_back(makeCallbackButton(u8"ㅤ", "dont_handle"));

    if ((b & RIGHT) != 0)
        keyboard[arrowsRow].push_back(makeCallbackButton(u8"▶️", utils::to_string(pageNo + 1))); // right
    else
        keyboard[arrowsRow].push_back(makeCallbackButton(u8"ㅤ", "dont_handle"));

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
constructMarkup(size_t pageNo, size_t numOfRecipesOnPage, api::models::recipe::RecipesList& recipesList) {
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

} // namespace

void renderCustomRecipesList(std::size_t pageNo, UserId userId, ChatId chatId, BotRef bot, RecipesApiRef recipesApi) {
    std::string pageInfo = utils::utf8str(u8"🔪 Рецепты созданные вами:");

    auto messageId = message::getMessageId(userId);

    const std::size_t numOfRecipesOnPage = 5;
    auto recipesList =
        recipesApi.getList(userId, PublicityFilterType::Custom, numOfRecipesOnPage, pageNo * numOfRecipesOnPage);

    if(recipesList.found == 0){
        pageInfo = utils::utf8str(u8"🔪 Вы находитесь в Мои рецепты. Создавайте и делитесь новыми рецептами.\n\n");
    }
    if (messageId) {
        bot.editMessageText(
            pageInfo, chatId, *messageId, makeKeyboardMarkup(constructMarkup(pageNo, numOfRecipesOnPage, recipesList)));
    }
}
} // namespace cookcookhnya::render::personal_account::recipes
