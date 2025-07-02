#pragma once

#include "backend/id_types.hpp"
#include "backend/models/recipe.hpp"
#include "render/common.hpp"
#include <vector>

namespace cookcookhnya::render::recipe_view {

struct textGenInfo {
    std::string text;
    bool isSuggestionMade{};
    std::unordered_set<api::StorageId> suggestedStorageIds;
    std::vector<std::string> foundInStoragesStrings;
    bool isAtLeastOneIngredientLack;
};

void renderStorageSuggestion(const std::vector<api::StorageId>& storageIdsToAccount,
                             api::RecipeId recipeId,
                             UserId userId,
                             ChatId chatId,
                             tg_types::MessageId messageId,
                             BotRef bot,
                             ApiClient api);

void renderRecipeViewAfterAddingStorage(const std::vector<api::StorageId>& storageIds,
                                        api::RecipeId recipeId,
                                        UserId userId,
                                        ChatId chatId,
                                        tg_types::MessageId messageId,
                                        BotRef bot,
                                        ApiClient api);
// Helper functions
std::vector<api::StorageId> storagesToShow(const std::vector<api::models::recipe::IngredientInRecipe>& ingredients,
                                           const std::vector<api::StorageId>& storageIdsToAccount);

textGenInfo textGen(const std::vector<api::StorageId>& storageIds,
                    const api::models::recipe::RecipeDetails& recipeIngredients,
                    UserId userId,
                    ApiClient api);

} // namespace cookcookhnya::render::recipe_view
