#include "backend/api/ingredients.hpp"

#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"
#include "utils/parsing.hpp"
#include "utils/to_string.hpp"

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

void IngredientsApi::putToStorage(UserId user, StorageId storage, IngredientId ingredient) const {
    jsonPutAuthed<void>(user, std::format("/my/storages/{}/ingredients/{}", storage, ingredient));
}

void IngredientsApi::deleteFromStorage(UserId user, StorageId storage, IngredientId ingredient) const {
    jsonDeleteAuthed<void>(user, std::format("/my/storages/{}/ingredients/{}", storage, ingredient));
}

Ingredient IngredientsApi::get(IngredientId ingredient) const {
    return jsonGet<Ingredient>(std::format("/ingredients/{}", ingredient));
}

IngredientSearchForStorageResponse IngredientsApi::searchForStorage(
    UserId user, std::string query, StorageId storage, std::size_t count, std::size_t offset) const {
    return jsonGetAuthed<IngredientSearchForStorageResponse>(user,
                                                             "/ingredients-for-storage",
                                                             {{"query", std::move(query)},
                                                              {"storage-id", utils::to_string(storage)},
                                                              {"size", utils::to_string(count)},
                                                              {"offset", utils::to_string(offset)}});
}

void IngredientsApi::putToRecipe(UserId user, RecipeId recipe, IngredientId ingredient) const {
    jsonPutAuthed<void>(user, std::format("/my/recipes/{}/ingredients/{}", recipe, ingredient));
}

void IngredientsApi::deleteFromRecipe(UserId user, RecipeId recipe, IngredientId ingredient) const {
    jsonDeleteAuthed<void>(user, std::format("/my/recipes/{}/ingredients/{}", recipe, ingredient));
}

IngredientSearchForRecipeResponse IngredientsApi::searchForRecipe(
    UserId user, std::string query, RecipeId recipe, std::size_t count, std::size_t offset) const {
    return jsonGetAuthed<IngredientSearchForRecipeResponse>(user,
                                                            "/ingredients-for-recipe",
                                                            {{"query", std::move(query)},
                                                             {"recipe-id", utils::to_string(recipe)},
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
    return utils::parse<IngredientId>(postWithJsonAuthed(user, "/my/ingredients", body));
}

void IngredientsApi::publishCustom(UserId user, IngredientId ingredient) const {
    jsonPostAuthed<void>(user, std::format("/my/ingredients/{}/publish", ingredient));
}

} // namespace cookcookhnya::api
