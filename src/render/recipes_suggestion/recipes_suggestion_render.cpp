#pragma once

#include "recipes_suggestion_render.hpp"
#include "render/common.hpp"
#include <format>
#include <string>
#include <utility>
#include <vector>

namespace cookcookhnya::render::recipes_suggestion {

InlineKeyboard constructMarkup(std::vector<StorageId> storages, int pageNo, StorageApiRef storageApi) {
    // CONSTANT AND SAME (STATIC) FOR EVERY USER (static const doesn't actually matter in this function was added
    // because of logic of that variable)
    static const int numOfRecipesOnPage = 10;
    auto recipesList =
        storageApi.getRecipes(std::move(storages), numOfRecipesOnPage, pageNo); // Take storages of user from backend
    int recipeId = 1;
    int amountOfRecipes = 11;
    bool ifMaxPage = amountOfRecipes - (numOfRecipesOnPage * pageNo) <= 0;
    // ONLY ONE CASE: WHEN First page has all recipes already
    if (pageNo == 1) {
        if (ifMaxPage) {
            // if didn't worked then the amount of recipes is less then i want to display -> no need for next or prev
            // button
            InlineKeyboard keyboard(1 + recipesList.size()); // 1 for back button return and other buttons are recipes
            for (uint32_t i = 0; i < recipesList.size(); i++) {
                // Print on button in form "1. {Recipe}"
                keyboard[i].push_back(
                    detail::makeCallbackButton(std::format("{}. {}",
                                                           1 + i + ((pageNo - 1) * numOfRecipesOnPage),
                                                           recipesList[i]),           // + 1 because i stars from 0
                                               std::format("recipe: {}", recipeId))); // HERE MUST BE RECIPE ID
            }

            /* Put the number of storages.
             * If more then one then return to storage list choose if one then go to the storage view.
             * Even if one storage was chosen in storage list choose it will return to view of these one storage.
             */
            keyboard[recipesList.size()].push_back(detail::makeCallbackButton(
                "Return", std::format("backFromSuggestedRecipes {}", storages.size()))); // To LAST row add "return"
            return keyboard;
        }

        // If first page wasn't didn't represent all recipes in one page then the field with arrows is required
        InlineKeyboard keyboard(
            2 + recipesList
                    .size()); // 1 for back button return, 1 for two arrows next and prev, and other buttons are recipes

        for (uint32_t i = 0; i < recipesList.size(); i++) {
            // Print on button in form "1. {Recipe}"
            keyboard[i].push_back(
                detail::makeCallbackButton(std::format("{}. {}",
                                                       1 + i + ((pageNo - 1) * numOfRecipesOnPage),
                                                       recipesList[i]),           // + 1 because i stars from 0
                                           std::format("recipe: {}", recipeId))); // HERE MUST BE RECIPE ID
        }
        // If pageNo == 1 and it's not 1st page then show only next button
        keyboard[recipesList.size()].push_back(detail::makeCallbackButton("Next page", std::to_string(pageNo + 1)));

        /* Put the number of storages.
         * If more then one then return to storage list choose if one then go to the storage view.
         * Even if one storage was chosen in storage list choose it will return to view of these one storage.
         */
        keyboard[recipesList.size() + 1].push_back(detail::makeCallbackButton(
            "Return", std::format("backFromSuggestedRecipes {}", storages.size()))); // To LAST row add "return"
        return keyboard;
    }

    // If first page wasn't didn't represent all recipes in one page then the field with arrows is required
    InlineKeyboard keyboard(
        2 +
        recipesList.size()); // 1 for back button return, 1 for two arrows next and prev, and other buttons are recipes
    for (uint32_t i = 0; i < recipesList.size(); i++) {
        // Print on button in form "1. {Recipe}"
        keyboard[i].push_back(
            detail::makeCallbackButton(std::format("{}. {}",
                                                   1 + i + ((pageNo - 1) * numOfRecipesOnPage),
                                                   recipesList[i]),           // + 1 because i stars from 0
                                       std::format("recipe: {}", recipeId))); // HERE MUST BE RECIPE ID
    }
    if (!ifMaxPage) {
        // Show both possible ways
        keyboard[recipesList.size()].reserve(2);
        keyboard[recipesList.size()].push_back(detail::makeCallbackButton("Previous page", std::to_string(pageNo - 1)));
        keyboard[recipesList.size()].push_back(detail::makeCallbackButton("Next page", std::to_string(pageNo + 1)));
    } else {
        // If pageNo == maxPage then it's last page -> won't show button next
        keyboard[recipesList.size()].push_back(detail::makeCallbackButton("Previous page", std::to_string(pageNo - 1)));
    }

    /* Put the number of storages.
     * If more then one then return to storage list choose if one then go to the storage view.
     * Even if one storage was chosen in storage list choose it will return to view of these one storage.
     */
    keyboard[recipesList.size() + 1].push_back(detail::makeCallbackButton(
        "Return", std::format("backFromSuggestedRecipes {}", storages.size()))); // To LAST row add "return"
    return keyboard;
}

void renderRecipesSuggestion(
    std::vector<StorageId> storages, int pageNo, ChatId chatId, BotRef bot, StorageApiRef storageApi) {

    std::string pageInfo = std::format("Page number {} \nRecipes we have chosen just for you:", pageNo);

    bot.sendMessage(chatId,
                    pageInfo,
                    nullptr,
                    nullptr,
                    detail::makeKeyboardMarkup(constructMarkup(std::move(storages), pageNo, storageApi)));
}

void editSuggestionMessage(std::vector<StorageId> storages,
                           int pageNo,
                           ChatId chatId,
                           MessageId messageId,
                           BotRef bot,
                           StorageApiRef storageApi) {
    std::string pageInfo = std::format("Page number {} \nRecipes we have chosen just for you:", pageNo);

    bot.editMessageText(pageInfo,
                        chatId,
                        messageId,
                        "",
                        "",
                        nullptr,
                        detail::makeKeyboardMarkup(constructMarkup(std::move(storages), pageNo, storageApi)));
}

} // namespace cookcookhnya::render::recipes_suggestion
