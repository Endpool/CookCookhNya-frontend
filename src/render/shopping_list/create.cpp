#include "create.hpp"

#include "backend/api/ingredients.hpp"
#include "backend/api/recipes.hpp"
#include "backend/id_types.hpp"
#include "message_tracker.hpp"
#include "render/common.hpp"
#include "utils.hpp"

#include <cmath>
#include <cstdint>
#include <format>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace cookcookhnya::render::shopping_list_create {

std::vector<api::IngredientId> renderShoppingListCreation(const std::vector<api::StorageId>& storageIds,
                                                          api::RecipeId recipeId,
                                                          UserId userId,
                                                          ChatId chatId,
                                                          BotRef bot,
                                                          RecipesApiRef recipesApi) {
    const std::unordered_set<api::StorageId> storageIdsSet(storageIds.begin(), storageIds.end());
    std::string toPrint = utils::utf8str(u8"Основываясь на недостающих ингредиентах, составили для вас продукты "
                                         u8"которые можно добавить в список покупок:\n *В самом низу выберите "
                                         u8"ингредиенты которые вы хотите исключить из списка покупок\n");

    auto ingredients = recipesApi.getIngredientsInRecipe(userId, recipeId).ingredients;
    std::vector<api::IngredientId> ingredientIds;
    std::vector<std::string> ingredientsName; // To minimise queries to backend
    bool isHavingIngredient = false;

    for (auto& ingredient : ingredients) { // Iterate through each ingredient
        isHavingIngredient = false;
        for (const api::StorageId storage : ingredient.inStorages) {
            if (storageIdsSet.contains(storage)) { // Then for this ingredient one of possible storages already selected
                isHavingIngredient = true;
                break; // No need to iterate further
            }
        }
        if (!isHavingIngredient) {
            ingredientIds.push_back(ingredient.id);
            ingredientsName.push_back(ingredient.name);
            toPrint += std::format(
                "- {}\n", ingredient.name); // Print only ingredients which are not in selected storages - вроде норм
        }
    }
    const int buttonRows = std::floor(((ingredientIds.size() + 1) / 2) + 2); // +1 for back, +1 for approve

    InlineKeyboard keyboard(buttonRows);
    uint64_t i = 0;
    for (auto ingredientId : ingredientIds) {
        const std::string& name =
            ingredientsName[i]; // NEED TO TEST if INGREDIENTS WILL MESS UP BETWEEN NAME AND ID - вроде норм
        if (i % 2 == 0) {
            keyboard[std::floor(i / 2)].reserve(2);
        }
        keyboard[std::floor(i / 2)].push_back(
            detail::makeCallbackButton(name, "i" + std::to_string(ingredientId))); // i stands for ingredient
        i++;
    }
    keyboard[std::floor((ingredientIds.size() + 1) / 2)].push_back(
        detail::makeCallbackButton(u8"▶️ Подтвердить", "confirm"));

    keyboard[std::floor(((ingredientIds.size() + 1) / 2) + 1)].push_back(
        detail::makeCallbackButton(u8"↩️ Назад", "back"));
    auto messageId = message::getMessageId(userId);
    if (messageId) {
        bot.editMessageText(
            toPrint, chatId, *messageId, "", "", nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
    }
    return ingredientIds;
}

void renderEditedShoppingListCreation(const std::vector<api::IngredientId>& ingredientIds,
                                      UserId userId,
                                      ChatId chatId,
                                      BotRef bot,
                                      IngredientsApiRef ingredientsApi) {
    std::vector<std::string> ingredientsName;
    std::string toPrint = utils::utf8str(u8"Основываясь на недостающих ингредиентах, составили для вас продукты "
                                         u8"которые можно добавить в список покупок:\n *В самом низу выберите "
                                         u8"ингредиенты которые вы хотите исключить из списка покупок\n");
    for (const api::IngredientId ingredientId : ingredientIds) {
        // IMPORTANT!: Probably can be optimized because this data is available at the recipe page
        // by Maxim Fomin
        std::string name = ingredientsApi.get(ingredientId)
                               .name; // NEED TO TEST if INGREDIENTS WILL MESS UP BETWEEN NAME AND ID - вроде норм
        ingredientsName.push_back(name);
        toPrint += std::format("- {}\n", name);
    }

    const int buttonRows = std::floor(((ingredientIds.size() + 1) / 2) + 2); // +1 for back

    InlineKeyboard keyboard(buttonRows);
    uint64_t i = 0;
    for (auto ingredientId : ingredientIds) {
        const std::string& name =
            ingredientsName[i]; // NEED TO TEST if INGREDIENTS WILL MESS UP BETWEEN NAME AND ID - вроде норм
        if (i % 2 == 0) {
            keyboard[std::floor(i / 2)].reserve(2);
        }
        keyboard[std::floor(i / 2)].push_back(
            detail::makeCallbackButton(name, "i" + std::to_string(ingredientId))); // i stands for ingredient
        i++;
    }

    keyboard[std::floor((ingredientIds.size() + 1) / 2)].push_back(
        detail::makeCallbackButton(u8"▶️ Подтвердить", "confirm"));

    keyboard[std::floor(((ingredientIds.size() + 1) / 2) + 1)].push_back(
        detail::makeCallbackButton(u8"↩️ Назад", "back"));
    auto messageId = message::getMessageId(userId);
    if (messageId) {
        bot.editMessageText(
            toPrint, chatId, *messageId, "", "", nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
    }
}

} // namespace cookcookhnya::render::shopping_list_create
