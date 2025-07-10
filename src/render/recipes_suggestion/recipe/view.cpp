#include "view.hpp"

#include "backend/id_types.hpp"
#include "backend/models/recipe.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <format>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace cookcookhnya::render::recipe_view {

textGenInfo textGen(const std::vector<api::StorageId>& storageIds,
                    const api::models::recipe::RecipeDetails& recipeIngredients,
                    UserId userId,
                    ApiClient api) { // will return needed text and some additional elements

    // Get two api's from apiClient
    auto storageApi = api.getStoragesApi();
    const std::unordered_set<api::StorageId> storageIdSet(storageIds.begin(), storageIds.end());

    std::unordered_set<api::StorageId> suggestedStorageIds;
    std::vector<std::string> foundInStoragesStrings;

    auto ingredients = recipeIngredients.ingredients;

    const std::string recipeName = recipeIngredients.name;
    std::string toPrint = utils::utf8str(u8"Ингредиенты для \"") + recipeName + "\"\n";
    std::vector<std::string> variants = {
        "\n", " и ", ", "}; // difference is 0 -> last, difference is 1 -> предпоследний.

    bool isContains = false;
    bool isSuggestionMade = false;
    bool isIngredientNotWritten = true;
    bool isAtLeastOneIngredientLack = false;
    size_t counterOfSuggestion = 0;
    for (auto& ingredient : ingredients) { // Iterate through each ingredient
        isIngredientNotWritten = true;
        isContains = false;
        if (ingredient.inStorages.empty()) {
            toPrint += std::format("- {}\n", ingredient.name);
            isAtLeastOneIngredientLack = true;
            continue;
        }

        for (size_t j = 0; j < ingredient.inStorages.size();
             j++) { // Iterate through each storage where ingredient is present
            if (storageIdSet.contains(
                    ingredient.inStorages[j])) { // If it contains then ingredient is in chosen storages
                toPrint += std::format("+ {}\n", ingredient.name);
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
                toPrint += std::format("? {}\n", ingredient.name);
                isIngredientNotWritten = false;

                foundInStoragesStrings.emplace_back(""); // New place for string for suggestion

                if (ingredient.inStorages.size() == 1) {

                    foundInStoragesStrings[counterOfSuggestion] += utils::utf8str(u8" *Найдено в хранилище: ");
                } else {
                    foundInStoragesStrings[counterOfSuggestion] += utils::utf8str(u8" *Найдено в хранилищах: ");
                }
            }
            auto storage = storageApi.get(userId, ingredient.inStorages[j]);

            suggestedStorageIds.insert(ingredient.inStorages[j]); // Keep set of storages which will be suggested
            foundInStoragesStrings[counterOfSuggestion] += // I felt myself genious after writing that line of code
                                                           // (here is one for, if-else and if technically)
                std::format("\"{}\"{}",
                            storage.name,
                            variants[std::min(variants.size() - 1, ingredient.inStorages.size() - j - 1)]);
        }
        counterOfSuggestion++; // If here then suggesiton was made
    }
    toPrint += std::format("Source Link: {}", recipeIngredients.link);
    return {.text = toPrint,
            .isSuggestionMade = isSuggestionMade,
            .suggestedStorageIds = suggestedStorageIds,
            .foundInStoragesStrings = foundInStoragesStrings,
            .isAtLeastOneIngredientLack =
                isAtLeastOneIngredientLack}; // Many info may be needed from that function to make right markup
}

void renderRecipeViewAfterAddingStorage(const std::vector<api::StorageId>& storageIds,
                                        api::RecipeId recipeId,
                                        UserId userId,
                                        ChatId chatId,
                                        BotRef bot,
                                        ApiClient api) {

    auto recipesApi = api.getRecipesApi();
    auto recipeIngredients = recipesApi.getIngredientsInRecipe(userId, recipeId);
    const textGenInfo text = textGen(storageIds, recipeIngredients, userId, api);

    const bool isSuggestionMade = text.isSuggestionMade;
    auto suggestedStorageIds = text.suggestedStorageIds;
    auto toPrint = text.text;
    const bool isAtLeastOneIngredientLack = text.isAtLeastOneIngredientLack;

    // if there is no lacking ingredients then there is no need to show field of shopping list
    const size_t buttonRows = isAtLeastOneIngredientLack ? 3 : 2;
    InlineKeyboard keyboard(buttonRows);

    keyboard[0].push_back(makeCallbackButton(u8"🧑‍🍳 Готовить", "cook"));
    if (isSuggestionMade) {
        std::string dataForSuggestion = "?";
        for (auto id : suggestedStorageIds) {
            dataForSuggestion += std::format("{} ", id);
        }
        keyboard[0].push_back(makeCallbackButton(u8"?", dataForSuggestion));
    }

    if (isAtLeastOneIngredientLack) {
        keyboard[1].push_back(makeCallbackButton(u8"📝 Составить список продуктов", "shopping_list"));
    }

    keyboard[buttonRows - 1].push_back(makeCallbackButton(u8"↩️ Назад", "back_to_suggestions"));
    auto messageId = message::getMessageId(userId);
    if (messageId) {
        // Only on difference between function above
        bot.editMessageText(toPrint, chatId, *messageId, makeKeyboardMarkup(std::move(keyboard)));
    }
}

std::vector<api::StorageId> storagesToShow(const std::vector<api::models::recipe::IngredientInRecipe>& ingredients,
                                           const std::vector<api::StorageId>& storageIdsToAccount) {
    std::vector<api::StorageId> storageIdsToShow;

    std::unordered_set<api::StorageId> toAdd; // If there will be only one element of storageId then remove
    // Iterate through each ingredient
    for (const auto& ingredient : ingredients) {
        bool isFound = false;
        // Iterate through each storage where ingredient is present
        for (const api::StorageId inStorage : ingredient.inStorages) {
            for (const api::StorageId stId : storageIdsToAccount) {
                if (stId == inStorage) {
                    isFound = true;
                    break;
                }
            }
            if (isFound)
                break;
        }
        if (!isFound) {
            // Proof that ingredient doesn't have "toxic" storages. Toxic storage is a storage which has some
            // ingredient so because of it other storages with that ingredient are not needed
            // But storages may be redeemed if they are in set of storages of ingredient where there is no toxic one
            for (const api::StorageId temp : ingredient.inStorages) {
                toAdd.insert(temp);
            }
        }
    }

    storageIdsToShow.reserve(toAdd.size());
    for (auto add : toAdd) {
        storageIdsToShow.push_back(add);
    }

    return storageIdsToShow;
}

void renderStorageSuggestion(const std::vector<api::StorageId>& storageIdsToAccount, // storages which are selected
                             api::RecipeId recipeId,
                             UserId userId,
                             ChatId chatId,
                             BotRef bot,
                             ApiClient api) {

    auto storageApi = api.getStoragesApi();

    auto recipesApi = api.getRecipesApi();
    auto recipeIngredients = recipesApi.getIngredientsInRecipe(userId, recipeId);
    auto ingredients = recipeIngredients.ingredients;

    const std::vector<api::StorageId> storageIdsToShow = storagesToShow(ingredients, storageIdsToAccount);

    const textGenInfo text = textGen(storageIdsToAccount, recipeIngredients, userId, api);
    auto toPrint = text.text;

    auto suggestionStrings = text.foundInStoragesStrings;
    size_t counterOfSuggestionsFound = 0;
    bool ifSuggestionEcountered = false;

    // This for can be moved to distinct function
    for (size_t i = 0; i < toPrint.size(); i++) { // Put suggestions here
        if (toPrint[i] == '\n' && ifSuggestionEcountered) {
            toPrint.insert(i + 1, suggestionStrings[counterOfSuggestionsFound]);
            counterOfSuggestionsFound++;
            ifSuggestionEcountered = false;
        }
        if (toPrint[i] == '?') {
            ifSuggestionEcountered = true;
        }
    }
    // This for is similar to suggested storages can be unionaized with this part of textGen (which will be incredibly
    // difficult to keep consistency of textGen fenction) To print storages which were added
    std::string storagesWhichAccount = utils::utf8str(u8"Выбранные хранилища: ");
    for (size_t i = 0; i < storageIdsToAccount.size(); i++) {
        auto storage = storageApi.get(userId, storageIdsToAccount[i]);
        storagesWhichAccount += std::format("\"{}\" ", storage.name);
        if (i == storageIdsToAccount.size() - 1) {
            storagesWhichAccount += "\n";
        }
    }
    toPrint.insert(0, storagesWhichAccount);
    const int buttonRows = std::floor(((storageIdsToShow.size() + 1) / 2) + 1); // +1 for back
    InlineKeyboard keyboard(buttonRows);

    uint64_t i = 0;
    for (auto storageId : storageIdsToShow) {
        const std::string name = storageApi.get(userId, storageId).name;
        if (i % 2 == 0) {
            keyboard[std::floor(i / 2)].reserve(2);
        }
        keyboard[std::floor(i / 2)].push_back(makeCallbackButton(name, "+" + std::to_string(storageId)));
        i++;
    }
    keyboard[std::floor((storageIdsToShow.size() + 1) / 2)].push_back(
        makeCallbackButton(u8"↩️ Назад", "back_to_recipe"));
    auto messageId = message::getMessageId(userId);
    if (messageId) {
        bot.editMessageText(toPrint, chatId, *messageId, "", "", nullptr, makeKeyboardMarkup(std::move(keyboard)));
    }
}
} // namespace cookcookhnya::render::recipe_view
