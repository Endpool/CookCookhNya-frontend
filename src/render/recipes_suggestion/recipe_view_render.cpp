#include "recipe_view_render.hpp"
#include "backend/models/recipe.hpp"
#include "render/common.hpp"
#include "utils.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <format>
#include <unordered_map>
#include <vector>

namespace cookcookhnya::render::recipe_view {

returnType textGen(std::vector<StorageId> const& storageIds,
                   const api::models::recipe::RecipeDetails& recipeIngredients,
                   UserId userId,
                   ApiClient api) { // will return needed text and some additional elements

    // Get two api's from apiClient
    auto storageApi = api.getStorages();
    std::unordered_set<StorageId> storageIdSet(storageIds.begin(), storageIds.end());
    std::unordered_set<StorageId> suggestedStorageIds;

    auto ingredients = recipeIngredients.ingredients;

    std::string recipeName = recipeIngredients.name;
    std::string toPrint = utils::utf8str(u8"Ингредиенты для \"") + recipeName + "\"\n";
    std::vector<std::string> variants = {
        "\n", " и ", ", "}; // difference is 0 -> last, difference is 1 -> предпоследний.

    bool isContains = false;
    bool isSuggestionMade = false;
    bool isIngredientNotWritten = true;

    for (auto& ingredient : ingredients) { // Iterate through each ingredient
        isIngredientNotWritten = true;
        isContains = false;
        if (ingredient.inStorages.size() == 0) {
            toPrint += std::format("{} -\n", ingredient.name);
            continue;
        }

        for (size_t j = 0; j < ingredient.inStorages.size();
             j++) { // Iterate through each storage where ingredient is present
            if (storageIdSet.contains(
                    ingredient.inStorages[j])) { // If it contains then ingredient is in chosen storages
                toPrint += std::format("{} +\n", ingredient.name);
                isContains = true;
                break;
            }
        }

        if (isContains) {
            continue;
        }

        for (size_t j = 0; j < ingredient.inStorages.size();
             j++) { // Iterate through each storage where ingredient is present
            isSuggestionMade = true;
            if (isIngredientNotWritten) {
                toPrint += std::format("{} ?\n", ingredient.name);
                isIngredientNotWritten = false;

                if (ingredient.inStorages.size() == 1) {
                    toPrint += utils::utf8str(u8" *Найдено в хранилище: ");
                } else {
                    toPrint += utils::utf8str(u8" *Найдено в хранилищах: ");
                }
            }
            auto storage = storageApi.get(userId, ingredient.inStorages[j]);

            suggestedStorageIds.insert(ingredient.inStorages[j]); // Keep set of storages which will be suggested
            toPrint += std::format("\"{}\"{}",
                                   storage.name,
                                   variants[std::min(variants.size() - 1, ingredient.inStorages.size() - j - 1)]);
        }
    }
    return {.text = toPrint, .isSuggestionMade = isSuggestionMade, .suggestedStorageIds = suggestedStorageIds};
}

void renderRecipeView(std::vector<StorageId> const& storageIds,
                      api::RecipeId recipeId,
                      UserId userId,
                      ChatId chatId,
                      BotRef bot,
                      ApiClient api) {

    auto recipesApi = api.getRecipes();
    auto recipeIngredients = recipesApi.getIngredientsInRecipe(userId, recipeId, storageIds);
    returnType text = textGen(storageIds, recipeIngredients, userId, api);

    bool isSuggestionMade = text.isSuggestionMade;
    auto suggestedStorageIds = text.suggestedStorageIds;
    auto toPrint = text.text;

    InlineKeyboard keyboard(3);
    keyboard[0].push_back(
        detail::makeCallbackButton(utils::utf8str(u8"Готовить"), "startCooking")); // Add needed info for next states!
    if (isSuggestionMade) {
        std::string dataForSuggestion = "?";
        for (auto id : suggestedStorageIds) {
            dataForSuggestion += std::format("{} ", id);
        }
        keyboard[0].push_back(detail::makeCallbackButton(utils::utf8str(u8"?"), dataForSuggestion));
    }

    keyboard[1].push_back(detail::makeCallbackButton(utils::utf8str(u8"Составить список продуктов"),
                                                     "makeReceipt")); // Add needed info for next states!
    keyboard[2].push_back(detail::makeCallbackButton(utils::utf8str(u8"Назад"), "backFromRecipeView"));

    bot.sendMessage(chatId, toPrint, nullptr, nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
}

void renderRecipeViewAfterAddingStorage(std::vector<StorageId> const& storageIds,
                                        api::RecipeId recipeId,
                                        UserId userId,
                                        ChatId chatId,
                                        tg_types::MessageId messageId,
                                        BotRef bot,
                                        ApiClient api) {
    auto recipesApi = api.getRecipes();
    auto recipeIngredients = recipesApi.getIngredientsInRecipe(userId, recipeId, storageIds);
    returnType text = textGen(storageIds, recipeIngredients, userId, api);

    bool isSuggestionMade = text.isSuggestionMade;
    auto suggestedStorageIds = text.suggestedStorageIds;
    auto toPrint = text.text;

    InlineKeyboard keyboard(3);
    keyboard[0].push_back(
        detail::makeCallbackButton(utils::utf8str(u8"Готовить"), "startCooking")); // Add needed info for next states!
    if (isSuggestionMade) {
        std::string dataForSuggestion = "?";
        for (auto id : suggestedStorageIds) {
            dataForSuggestion += std::format("{} ", id);
        }
        keyboard[0].push_back(detail::makeCallbackButton(utils::utf8str(u8"?"), dataForSuggestion));
    }

    keyboard[1].push_back(detail::makeCallbackButton(utils::utf8str(u8"Составить список продуктов"),
                                                     "makeReceipt")); // Add needed info for next states!
    keyboard[2].push_back(detail::makeCallbackButton(utils::utf8str(u8"Назад"), "backFromRecipeView"));

    bot.editMessageText(toPrint,
                        chatId,
                        messageId,
                        "",
                        "",
                        nullptr,
                        detail::makeKeyboardMarkup(std::move(keyboard))); // Only on difference between function above
}

void renderStorageSuggestion(std::vector<StorageId>& storageIdsToShow,
                             std::vector<StorageId>& storageIdsToAccount, // storages which are selected
                             api::RecipeId recipeId,
                             UserId userId,
                             ChatId chatId,
                             tg_types::MessageId messageId,
                             BotRef bot,
                             ApiClient api) {

    auto storageApi = api.getStorages();
    storageIdsToShow.clear();
    std::unordered_set<StorageId> toAdd; // If there will be only one element of storageId then remove
    std::vector<StorageId> toRemove;
    auto recipesApi = api.getRecipes();
    auto recipeIngredients = recipesApi.getIngredientsInRecipe(userId, recipeId, storageIdsToAccount);
    auto ingredients = recipeIngredients.ingredients;
    bool isFound = false;
    for (auto& ingredient : ingredients) {
        isFound = false;                                    // Iterate through each ingredient
        for (StorageId inStorage : ingredient.inStorages) { // Iterate through each storage where ingredient is present
            for (StorageId i : storageIdsToAccount) {
                if (i == inStorage) {
                    toRemove.insert(toRemove.end(), ingredient.inStorages.begin(), ingredient.inStorages.end());
                    isFound = true;
                    break;
                }
            }
            if (isFound) {
                break;
            }
        }
        if (!isFound) {
            // Proof that ingredient doesn't have "toxic" storages. Toxic storage is a storage which has some
            // ingredient so because of it other storages with that ingredient are not needed
            // But storages may be redeemed if they are in set of storages of ingredient where there is no toxic one
            for (StorageId temp : ingredient.inStorages) {
                toAdd.insert(temp);
            }
        }
    }

    for (size_t i = 0; i < storageIdsToShow.size(); i++) {
        for (StorageId j : toRemove) {
            if (storageIdsToShow[i] == j) {
                storageIdsToShow.erase(storageIdsToShow.begin() + i, storageIdsToShow.begin() + i + 1);
            }
        }
    }
    for (auto add : toAdd) {
        storageIdsToShow.push_back(add);
    }
    returnType text = textGen(storageIdsToAccount, recipeIngredients, userId, api);
    auto toPrint = text.text;
    std::vector<std::string> storageNames;

    size_t buttonRows = 0;
    buttonRows = std::floor(storageIdsToShow.size() + 1) / 2 + 1; // +1 for back
    InlineKeyboard keyboard(buttonRows);

    uint64_t i = 0;

    std::string dataToShow = "+";
    for (auto id : storageIdsToShow) {
        dataToShow += std::format("{} ", id);
    }

    for (auto storageId : storageIdsToShow) {
        std::string name = storageApi.get(userId, storageId).name;
        if (i % 2 == 0) {
            keyboard[std::floor(i / 2)].reserve(2);
        }
        keyboard[std::floor(i / 2)].push_back(detail::makeCallbackButton(name, dataToShow + std::to_string(storageId)));
        i++;
    }
    keyboard[std::floor(storageIdsToShow.size() + 1) / 2].push_back(
        detail::makeCallbackButton(utils::utf8str(u8"Назад"), "BackFromAddingStorages"));
    bot.editMessageText(toPrint, chatId, messageId, "", "", nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
}
} // namespace cookcookhnya::render::recipe_view
