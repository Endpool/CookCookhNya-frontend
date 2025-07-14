#include "add_storage.hpp"

#include "backend/id_types.hpp"
#include "backend/models/recipe.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils/utils.hpp"
#include "view.hpp" // Reuse textGen from here

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <format>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace cookcookhnya::render::recipe {

std::vector<api::StorageId> storagesToShow(const std::vector<api::models::recipe::IngredientInRecipe>& ingredients,
                                           const std::vector<api::StorageId>& storageIdsToAccount) {
    std::vector<api::StorageId> storageIdsToShow;

    std::unordered_set<api::StorageId> toAdd; // If there will be only one element of storageId then remove
    bool isFound = false;
    for (const auto& ingredient : ingredients) {
        isFound = false; // Iterate through each ingredient
        for (const api::StorageId inStorage :
             ingredient.inStorages) { // Iterate through each storage where ingredient is present
            for (const api::StorageId stId : storageIdsToAccount) {
                if (stId == inStorage) {
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

void renderStoragesSuggestion(const std::vector<api::StorageId>& storageIdsToAccount, // storages which are selected
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
        keyboard[std::floor(i / 2)].push_back(makeCallbackButton(name, "+" + utils::to_string(storageId)));
        i++;
    }
    keyboard[std::floor((storageIdsToShow.size() + 1) / 2)].push_back(
        makeCallbackButton(u8"↩️ Назад", "back_from_adding_storages"));
    auto messageId = message::getMessageId(userId);
    if (messageId) {
        bot.editMessageText(toPrint, chatId, *messageId, "", "", nullptr, makeKeyboardMarkup(std::move(keyboard)));
    }
}
} // namespace cookcookhnya::render::recipe
