#pragma once

#include "backend/id_types.hpp"
#include "backend/models/recipe.hpp"
#include "render/common.hpp"

#include <string>
#include <vector>

namespace cookcookhnya::render::recipe {

struct textGenInfo {
    std::string text;
    bool isSuggestionMade{};
    std::unordered_set<api::StorageId> suggestedStorageIds;
    std::vector<std::string> foundInStoragesStrings;
    bool isAtLeastOneIngredientLack;
};

void renderRecipeView(const std::vector<api::StorageId>& storageIds,
                      api::RecipeId recipeId,
                      UserId userId,
                      ChatId chatId,
                      BotRef bot,
                      ApiClient api);
// Helper functions

textGenInfo textGen(const std::vector<api::StorageId>& storageIds,
                    const api::models::recipe::RecipeDetails& recipeIngredients,
                    UserId userId,
                    ApiClient api);

} // namespace cookcookhnya::render::recipe
