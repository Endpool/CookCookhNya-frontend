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
#include <ranges>
#include <string>
#include <vector>

namespace cookcookhnya::render::recipes_suggestions {

using namespace api::models::storage;
using namespace api::models::recipe;
using namespace std::views;
using namespace std::ranges;

namespace {

// offset is variable which defines amout of rows before beggining of paging
// fullKeyBoardSize is self explanatory
InlineKeyboard constructNavigationsMarkup(std::size_t offset,
                                          std::size_t fullKeyBoardSize,
                                          std::size_t pageNo,
                                          std::size_t numOfRecipesOnPage,
                                          RecipesListWithIngredientsCount recipesList) {
    const std::size_t amountOfRecipes = recipesList.found;
    std::size_t maxPageNum = std::ceil(static_cast<double>(amountOfRecipes) / static_cast<double>(numOfRecipesOnPage));

    const size_t recipesToShow = std::min(numOfRecipesOnPage, recipesList.page.size());
    // + 1 because of the 0-indexing, as comparisson is between num of recipes gotten and that
    // will be actually shown
    const bool lastPage =
        static_cast<int>(amountOfRecipes) - static_cast<int>(numOfRecipesOnPage) * (static_cast<int>(pageNo) + 1) <= 0;

    if (offset + recipesToShow > fullKeyBoardSize) { // IN ERROR HANDLING MAY USE ASSERT
        InlineKeyboard error(0);
        return error;
    }
    const std::size_t arrowsRow = offset + recipesToShow;

    InlineKeyboard keyboard(pageNo == 0 && lastPage ? fullKeyBoardSize - 1 : fullKeyBoardSize);
    int counter = 0;
    for (std::size_t i = 0; i < recipesToShow; i++) {
        keyboard[i + offset].push_back(makeCallbackButton(std::format("{}. {} [{} из {}]",
                                                                      1 + counter + ((pageNo)*numOfRecipesOnPage),
                                                                      recipesList.page[counter].name,
                                                                      recipesList.page[counter].available,
                                                                      recipesList.page[counter].total),
                                                          std::format("recipe_{}", recipesList.page[counter].id)));
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

InlineKeyboard constructOnlyBack() {
    InlineKeyboard keyboard(1);
    keyboard[0].push_back(makeCallbackButton(u8"↩️ Назад", "back"));
    return keyboard;
}

InlineKeyboard
constructMarkup(std::size_t pageNo, std::size_t numOfRecipesOnPage, RecipesListWithIngredientsCount& recipesList) {
    const std::size_t numOfRows = 2; // 1 for back button return, 1 for arrows (ALWAYS ACCOUNT ARROWS)
    const std::size_t offset = 0;    // Number of rows before list
    const std::size_t recipesToShow = std::min(numOfRecipesOnPage, recipesList.page.size());

    InlineKeyboard keyboard =
        recipesList.found == 0
            ? constructOnlyBack()
            : constructNavigationsMarkup(offset, numOfRows + recipesToShow, pageNo, numOfRecipesOnPage, recipesList);
    if (keyboard.empty()) { // If error happened ADD PROPER ERROR HANDLING IF FUNCTION WILL BE REUSED
        return keyboard;
    }

    return keyboard;
}

} // namespace

void renderRecipesSuggestion(std::vector<StorageSummary>& storages,
                             std::size_t pageNo,
                             UserId userId,
                             ChatId chatId,
                             BotRef bot,
                             RecipesApiRef recipesApi) {
    std::string pageInfo = utils::utf8str(u8"🔪 Рецепты подобранные специально для вас");
    const std::size_t numOfRecipesOnPage = 5;
    const std::size_t numOfRecipes = 500;

    auto storagesIds = storages | views::transform(&StorageSummary::id) | to<std::vector>();
    auto recipesList = recipesApi.getSuggestedRecipes(userId, storagesIds, numOfRecipes, pageNo * numOfRecipesOnPage);
    
    if(recipesList.found == 0){
        pageInfo = utils::utf8str(u8"😔 К сожалению, нам не удалось найти подходящие рецепты для вас...");
    }
    if (auto messageId = message::getMessageId(userId)) {
        bot.editMessageText(
            pageInfo, chatId, *messageId, makeKeyboardMarkup(constructMarkup(pageNo, numOfRecipesOnPage, recipesList)));
    } else {
        auto message = bot.sendMessage(chatId, pageInfo, makeKeyboardMarkup(constructMarkup(pageNo, numOfRecipesOnPage, recipesList)));
        message::addMessageId(userId, message->messageId);
    }
}

} // namespace cookcookhnya::render::recipes_suggestions
