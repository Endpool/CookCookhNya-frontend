#include "recipes_suggestion_render.hpp"

#include "backend/id_types.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils.hpp"

#include <cstddef>
#include <format>
#include <string>
#include <vector>

namespace cookcookhnya::render::recipes_suggestion {

InlineKeyboard
constructMarkup(const std::vector<api::StorageId>& storageIds, int pageNo, UserId userId, RecipesApiRef recipesApi) {

    // CONSTANT AND SAME (STATIC) FOR EVERY USER (static const doesn't actually matter in this function was added
    // because of logic of that variable)
    static const int numOfRecipesOnPage = 5;

    auto recipesList = recipesApi.getRecipeList(userId,
                                                numOfRecipesOnPage,
                                                (pageNo)*numOfRecipesOnPage,
                                                storageIds); // Take storages of user from backend

    const int amountOfRecipes = recipesList.recipesFound;
    const bool ifMaxPage = amountOfRecipes - (numOfRecipesOnPage * (pageNo + 1)) <=
                           0; // + 1 because of the 0-indexing, as comparisson is between num of recipes gotten and that
                              // will be actually shown
    // ONLY ONE CASE: WHEN First page has all recipes already
    if (pageNo == 0) {
        if (ifMaxPage) {
            // if didn't worked then the amount of recipes is less then i want to display -> no need for next or prev
            // button
            InlineKeyboard keyboard(
                1 + recipesList.recipesPage.size()); // 1 for back button return and other buttons are recipes
            for (std::size_t i = 0; i < recipesList.recipesPage.size(); i++) {
                // Print on button in form "1. {Recipe}"
                keyboard[i].push_back(detail::makeCallbackButton(
                    std::format("{}. {} [{}/{}]",
                                1 + i + ((static_cast<std::size_t>(pageNo)) * numOfRecipesOnPage),
                                recipesList.recipesPage[i].name,
                                recipesList.recipesPage[i].available,
                                recipesList.recipesPage[i].total),              // + 1 because i stars from 0
                    std::format("recipe: {}", recipesList.recipesPage[i].id))); // RECIPE ID
            }

            /* Put the number of storages.
             * If more then one then return to storage list choose if one then go to the storage view.
             * Even if one storage was chosen in storage list choose it will return to view of these one storage.
             */
            keyboard[recipesList.recipesPage.size()].push_back(detail::makeCallbackButton(
                utils::utf8str(u8"↩️ Назад"),
                std::format("backFromSuggestedRecipes {}", storageIds.size()))); // To LAST row add "return"
            return keyboard;
        }

        // If first page wasn't didn't represent all recipes in one page then the field with arrows is required
        // 1 for back button return, 1 for two arrows next and prev, and other buttons are recipes
        InlineKeyboard keyboard(2 + recipesList.recipesPage.size());

        for (std::size_t i = 0; i < recipesList.recipesPage.size(); i++) {
            // Print on button in form "1. {Recipe}"
            keyboard[i].push_back(detail::makeCallbackButton(
                std::format("{}. {} [{}/{}]",
                            1 + i + ((static_cast<std::size_t>(pageNo)) * numOfRecipesOnPage),
                            recipesList.recipesPage[i].name,
                            recipesList.recipesPage[i].available,
                            recipesList.recipesPage[i].total),              // + 1 because i stars from 0
                std::format("recipe: {}", recipesList.recipesPage[i].id))); // RECIPE ID
        }
        // If pageNo == 0 and it's not 1st page then show only next button
        keyboard[recipesList.recipesPage.size()].push_back(detail::makeCallbackButton("⏭️", std::to_string(pageNo + 1)));

        /* Put the number of storages.
         * If more then one then return to storage list choose if one then go to the storage view.
         * Even if one storage was chosen in storage list choose it will return to view of these one storage.
         */
        keyboard[recipesList.recipesPage.size() + 1].push_back(detail::makeCallbackButton(
            utils::utf8str(u8"↩️ Назад"),
            std::format("backFromSuggestedRecipes {}", storageIds.size()))); // To LAST row add "return"
        return keyboard;
    }

    // If first page wasn't didn't represent all recipes in one page then the field with arrows is required
    // 1 for back button return, 1 for two arrows next and prev, and other buttons are recipes
    InlineKeyboard keyboard(2 + recipesList.recipesPage.size());
    for (std::size_t i = 0; i < recipesList.recipesPage.size(); i++) {
        // Print on button in form "1. {Recipe}"
        keyboard[i].push_back(
            detail::makeCallbackButton(std::format("{}. {} [{}/{}]",
                                                   1 + i + ((static_cast<std::size_t>(pageNo)) * numOfRecipesOnPage),
                                                   recipesList.recipesPage[i].name,
                                                   recipesList.recipesPage[i].available,
                                                   recipesList.recipesPage[i].total), // + 1 because i stars from 0
                                       std::format("recipe: {}", recipesList.recipesPage[i].id))); // RECIPE ID
    }
    if (!ifMaxPage) {
        // Show both possible ways
        keyboard[recipesList.recipesPage.size()].reserve(2);
        keyboard[recipesList.recipesPage.size()].push_back(
            detail::makeCallbackButton(utils::utf8str(u8"⏮️"), std::to_string(pageNo - 1)));
        keyboard[recipesList.recipesPage.size()].push_back(
            detail::makeCallbackButton(utils::utf8str(u8"⏭️"), std::to_string(pageNo + 1)));
    } else {
        // If pageNo == maxPage then it's last page -> won't show button next
        keyboard[recipesList.recipesPage.size()].push_back(
            detail::makeCallbackButton(utils::utf8str(u8"⏮️"), std::to_string(pageNo - 1)));
    }

    /* Put the number of storages.
     * If more then one then return to storage list choose if one then go to the storage view.
     * Even if one storage was chosen in storage list choose it will return to view of these one storage.
     */
    keyboard[recipesList.recipesPage.size() + 1].push_back(detail::makeCallbackButton(

        utils::utf8str(u8"↩️ Назад"),
        std::format("backFromSuggestedRecipes {}", storageIds.size()))); // To LAST row add "return"
    return keyboard;
}

void editRecipesSuggestion(const std::vector<api::StorageId>& storageIds,
                           int pageNo,
                           UserId userId,
                           ChatId chatId,
                           BotRef bot,
                           RecipesApiRef recipesApi) {
    const std::string pageInfo = utils::utf8str(u8"🔢 Номер страницы: ") + std::to_string(pageNo) +
                                 utils::utf8str(u8"\n🔪 Рецепты подобранные специально для вас");

    auto messageId = message::getMessageId(userId);
    if (messageId) {
        bot.editMessageText(pageInfo,
                            chatId,
                            *messageId,
                            "",
                            "",
                            nullptr,
                            detail::makeKeyboardMarkup(constructMarkup(storageIds, pageNo, userId, recipesApi)));
    }
}

} // namespace cookcookhnya::render::recipes_suggestion
