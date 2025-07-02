#include "render/shopping_list/shopping_list_creation_render.hpp"
#include "backend/api/ingredients.hpp"
#include "backend/api/recipes.hpp"
#include "backend/id_types.hpp"
#include "render/common.hpp"
#include <cstddef>
#include <format>
#include <unordered_set>
#include <vector>

namespace cookcookhnya::render::shopping_list_creation {

std::vector<api::IngredientId> renderShoppingListCreation(const std::vector<api::StorageId>& storageIds,
                                                          api::RecipeId recipeId,
                                                          UserId userId,
                                                          ChatId chatId,
                                                          BotRef bot,
                                                          ApiClient api) {
    auto recipesApi = api.getRecipes();
    auto ingredientsApi = api.getIngredients();
    std::unordered_set<api::StorageId> storageIdsSet(storageIds.begin(), storageIds.end());
    std::string toPrint = utils::utf8str(u8"Основываясь на недостающих ингредиентах, составили для вас продукты "
                                         u8"которые можно добавить в список покупок:\n");

    auto ingredients = recipesApi.getIngredientsInRecipe(userId, recipeId).ingredients;
    std::vector<api::IngredientId> ingredientIds;
    std::vector<std::string> ingredientsName; // To minimise queries to backend
    bool isHavingIngredient = false;

    for (auto& ingredient : ingredients) { // Iterate through each ingredient
        isHavingIngredient = false;
        for (api::StorageId storage : ingredient.inStorages) {
            if (storageIdsSet.contains(storage)) { // Then for this ingredient one of possible storages already selected
                isHavingIngredient = true;
                break; // No need to iterate further
            }
        }
        if (!isHavingIngredient) {
            ingredientIds.push_back(ingredient.id);
            ingredientsName.push_back(ingredient.name);
            toPrint +=
                std::format("- {}\n", ingredient.name); // Print only ingredients which are not in selected storages
        }
    }
    int buttonRows = std::floor(((ingredientIds.size() + 1) / 2) + 1); // +1 for back

    InlineKeyboard keyboard(buttonRows);
    uint64_t i = 0;
    for (auto ingredientId : ingredientIds) {
        const std::string& name = ingredientsName[i]; // NEED TO TEST if INGREDIENTS WILL MESS UP BETWEEN NAME AND ID
        if (i % 2 == 0) {
            keyboard[std::floor(i / 2)].reserve(2);
        }
        keyboard[std::floor(i / 2)].push_back(
            detail::makeCallbackButton(name, "i" + std::to_string(ingredientId))); // i stands for ingredient
        i++;
    }
    keyboard[std::floor((ingredientIds.size() + 1) / 2)].push_back(
        detail::makeCallbackButton(utils::utf8str(u8"Назад"), "BackFromShoppingList"));

    bot.sendMessage(chatId, toPrint, nullptr, nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
    return ingredientIds;
}

void renderEditedShoppingListCreation(std::vector<api::IngredientId> ingredientIds,
                                      api::IngredientId ingredientIdToRemove,
                                      ChatId chatId,
                                      tg_types::MessageId messageId,
                                      BotRef bot,
                                      IngredientsApiRef ingredientsApi) {
    std::vector<std::string> ingredientsName;
    for (auto ingredientId = ingredientIds.begin(); ingredientId < ingredientIds.end(); ingredientId++) {
        if (*ingredientId == ingredientIdToRemove) {
            ingredientIds.erase(ingredientId);
        }
    }
    std::string toPrint = utils::utf8str(u8"Основываясь на недостающих ингредиентах, составили для вас продукты "
                                         u8"которые можно добавить в список покупок:\n");
    for (api::IngredientId ingredientId : ingredientIds) {
        std::string name =
            ingredientsApi.get(ingredientId).name; // NEED TO TEST if INGREDIENTS WILL MESS UP BETWEEN NAME AND ID
        ingredientsName.push_back(name);
        toPrint += std::format("- {}\n", name);
    }

    int buttonRows = std::floor(((ingredientIds.size() + 1) / 2) + 1); // +1 for back

    InlineKeyboard keyboard(buttonRows);
    uint64_t i = 0;
    for (auto ingredientId : ingredientIds) {
        const std::string& name = ingredientsName[i]; // NEED TO TEST if INGREDIENTS WILL MESS UP BETWEEN NAME AND ID
        if (i % 2 == 0) {
            keyboard[std::floor(i / 2)].reserve(2);
        }
        keyboard[std::floor(i / 2)].push_back(
            detail::makeCallbackButton(name, "i" + std::to_string(ingredientId))); // i stands for ingredient
        i++;
    }
    keyboard[std::floor((ingredientIds.size() + 1) / 2)].push_back(
        detail::makeCallbackButton(utils::utf8str(u8"Назад"), "BackFromShoppingList"));

    bot.editMessageText(toPrint, chatId, messageId, "", "", nullptr, detail::makeKeyboardMarkup(std::move(keyboard)));
}

} // namespace cookcookhnya::render::shopping_list_creation
