#pragma once

#include "backend/id_types.hpp"
#include "backend/models/recipe.hpp"
#include "render/common.hpp"
#include <vector>

namespace cookcookhnya::render::recipe_view {

struct returnType {
    std::string text;
    bool isSuggestionMade{};
    std::unordered_set<StorageId> suggestedStorageIds;
};

void renderRecipeView(
    std::vector<StorageId> const& storageIds, // Will be deleted when all messages will be from editing - for now it's
                                              // entry point and guarantee that message exists
    api::RecipeId recipeId,
    UserId userId,
    ChatId chatId,
    BotRef bot,
    ApiClient api);

void renderStorageSuggestion(std::vector<StorageId>& storageIdsToShow,
                             std::vector<StorageId>& storageIdsToAccount,
                             api::RecipeId recipeId,
                             UserId userId,
                             ChatId chatId,
                             tg_types::MessageId messageId,
                             BotRef bot,
                             ApiClient api);

void renderRecipeViewAfterAddingStorage(std::vector<StorageId> const& storageIds,
                                        api::RecipeId recipeId,
                                        UserId userId,
                                        ChatId chatId,
                                        tg_types::MessageId messageId,
                                        BotRef bot,
                                        ApiClient api);

returnType textGen(std::vector<StorageId> const& storageIds,
                   const api::models::recipe::RecipeDetails& recipeIngredients,
                   UserId userId,
                   ApiClient api);
} // namespace cookcookhnya::render::recipe_view
