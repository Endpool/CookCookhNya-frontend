#include "backend/api/ingredients.hpp"

#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"

#include <cstddef>
#include <format>
#include <string>
#include <utility>
#include <vector>

namespace cookcookhnya::api {

using namespace models::ingredient;

std::vector<Ingredient> IngredientsApi::getStorageIngredients(UserId user, StorageId storage) const {
    return jsonGetAuthed<std::vector<Ingredient>>(user, std::format("/my/storages/{}/ingredients", storage));
}

void IngredientsApi::putToStorage(UserId user, StorageId storage, IngredientId id) const {
    jsonPutAuthed<void>(user, std::format("/my/storages/{}/ingredients/{}", storage, id));
}

void IngredientsApi::deleteFromStorage(UserId user, StorageId storage, IngredientId id) const {
    jsonDeleteAuthed<void>(user, std::format("/my/storages/{}/ingredients/{}", storage, id));
}

Ingredient IngredientsApi::get(IngredientId id) const {
    return jsonGet<Ingredient>(std::format("/ingredients/{}", id));
}

IngredientSearchForStorageResponse IngredientsApi::searchForStorage(
    UserId userId, std::string query, StorageId storage, std::size_t count, std::size_t offset) const {
    return jsonGetAuthed<IngredientSearchForStorageResponse>(userId,
                                                             "/ingredients-for-storage",
                                                             {{"query", std::move(query)},
                                                              {"storage-id", utils::to_string(storage)},
                                                              {"size", utils::to_string(count)},
                                                              {"offset", utils::to_string(offset)}});
}

void IngredientsApi::putToRecipe(UserId user, RecipeId recipeId, IngredientId id) const {
    jsonPutAuthed<void>(user,
                        std::format("/my/recipes/{}/ingredients/{}", recipeId, id)); // path analogically to ingredients
}

void IngredientsApi::deleteFromRecipe(UserId user, RecipeId recipeId, IngredientId id) const {
    jsonDeleteAuthed<void>(user, std::format("/my/recipes/{}/ingredients/{}", recipeId, id));
}

IngredientSearchForRecipeResponse IngredientsApi::searchForRecipe(
    UserId userId, std::string query, RecipeId recipeId, std::size_t count, std::size_t offset) const {
    return jsonGetAuthed<IngredientSearchForRecipeResponse>(userId,
                                                            "/ingredients-for-recipe",
                                                            {{"query", std::move(query)},
                                                             {"recipe-id", utils::to_string(recipeId)},
                                                             {"size", utils::to_string(count)},
                                                             {"offset", utils::to_string(offset)}});
}

IngredientSearchResponse
IngredientsApi::search(std::string query, std::size_t size, std::size_t offset, std::size_t threshold) const {
    return jsonGet<IngredientSearchResponse>("/ingredients",
                                             {},
                                             {{"query", std::move(query)},
                                              {"size", utils::to_string(size)},
                                              {"offset", utils::to_string(offset)},
                                              {"threshold", utils::to_string(threshold)}});
}

std::vector<Ingredient> IngredientsApi::getCustomIngredients(UserId user) const {
    return jsonGetAuthed<std::vector<Ingredient>>(user, "/my/ingredients/");
}

IngredientId IngredientsApi::createCustom(UserId user, const IngredientCreateBody& body) const {
    return jsonPostWithJsonAuthed<IngredientId>(user, "/my/ingredients", body);
}

void IngredientsApi::publishCustom(UserId user, IngredientId id) const {
    jsonPostAuthed<void>(user, std::format("/my/ingredients/{}/publish", id));
}

} // namespace cookcookhnya::api
