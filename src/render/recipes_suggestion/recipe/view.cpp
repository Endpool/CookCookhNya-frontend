#include "view.hpp"

#include "backend/id_types.hpp"
#include "backend/models/recipe.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
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

    const size_t buttonRows =
        isAtLeastOneIngredientLack
            ? 3
            : 2; // if there is no lacking ingredients then there is no need to show field of shopping list
    InlineKeyboard keyboard(buttonRows);

    keyboard[0].push_back(detail::makeCallbackButton(u8"🧑‍🍳 Готовить",
                                                     "start_cooking")); // Add needed info for next states!
    if (isSuggestionMade) {
        std::string dataForSuggestion = "?";
        for (auto id : suggestedStorageIds) {
            dataForSuggestion += std::format("{} ", id);
        }
        keyboard[0].push_back(detail::makeCallbackButton(u8"?", dataForSuggestion));
    }

    if (isAtLeastOneIngredientLack) {
        keyboard[1].push_back(detail::makeCallbackButton(u8"📝 Составить список продуктов",
                                                         "make_product_list")); // Add needed info for next states!
    }

    keyboard[buttonRows - 1].push_back(detail::makeCallbackButton(u8"↩️ Назад", "back_from_recipe_view"));
    auto messageId = message::getMessageId(userId);
    if (messageId) {
        bot.editMessageText(
            toPrint,
            chatId,
            *messageId,
            "",
            "",
            nullptr,
            detail::makeKeyboardMarkup(std::move(keyboard))); // Only on difference between function above
    }
}

} // namespace cookcookhnya::render::recipe_view
