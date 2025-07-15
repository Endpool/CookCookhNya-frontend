#include "backend/api/ingredients.hpp"

#include "backend/api/common.hpp"
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

// /storages/{storageId}/ingredients
std::vector<Ingredient> IngredientsApi::getStorageIngredients(UserId user, StorageId storage) const {
    return jsonGetAuthed<std::vector<Ingredient>>(user, std::format("/storages/{}/ingredients", storage));
}

// /storages/{storageId}/ingredients/{ingredientId}
void IngredientsApi::putToStorage(UserId user, StorageId storage, IngredientId ingredient) const {
    jsonPutAuthed<void>(user, std::format("/storages/{}/ingredients/{}", storage, ingredient));
}

// /storages/{storageId}/ingredients/{ingredientId}
void IngredientsApi::deleteFromStorage(UserId user, StorageId storage, IngredientId ingredient) const {
    jsonDeleteAuthed<void>(user, std::format("/storages/{}/ingredients/{}", storage, ingredient));
}

// /storages/{storageId}/ingredients
void IngredientsApi::deleteMultipleFromStorage(UserId user,
                                               StorageId storage,
                                               const std::vector<IngredientId>& ingredients) const {
    httplib::Params params = {};
    for (auto id : ingredients)
        params.insert({"ingredient", utils::to_string(id)});
    jsonDeleteAuthed<void>(user, std::format("/storages/{}/ingredients/", storage), params);
}

// /ingredients-for-storage
IngredientSearchForStorageResponse IngredientsApi::searchForStorage(
    UserId user, std::string query, StorageId storage, std::size_t count, std::size_t offset) const {
    return jsonGetAuthed<IngredientSearchForStorageResponse>(user,
                                                             "/ingredients-for-storage",
                                                             {{"query", std::move(query)},
                                                              {"storage-id", utils::to_string(storage)},
                                                              {"size", utils::to_string(count)},
                                                              {"offset", utils::to_string(offset)}});
}

// /public/ingredients
IngredientSearchResponse
IngredientsApi::publicSearch(std::string query, std::size_t size, std::size_t offset, std::size_t threshold) const {
    return jsonGet<IngredientSearchResponse>("/public/ingredients",
                                             {},
                                             {{"query", std::move(query)},
                                              {"threshold", utils::to_string(threshold)},
                                              {"size", utils::to_string(size)},
                                              {"offset", utils::to_string(offset)}});
}

// /ingredients
IngredientSearchResponse IngredientsApi::search(UserId user,
                                                std::string query,
                                                std::size_t threshold,
                                                std::size_t size,
                                                std::size_t offset,
                                                filterType filter) const {
    return jsonGetAuthed<IngredientSearchResponse>(user,
                                                   "/ingredients",
                                                   {{"query", std::move(query)},
                                                    {"size", utils::to_string(size)},
                                                    {"offset", utils::to_string(offset)},
                                                    {"threshold", utils::to_string(threshold)},
                                                    {"filter", filterStr(filter)}});
}

// /public/ingredients/{ingredientId}
Ingredient IngredientsApi::getPublicIngredient(IngredientId ingredient) const {
    return jsonGet<Ingredient>(std::format("/public/ingredients/{}", ingredient));
}

// /recipes/{recipeId}/ingredients/{ingredientId}
void IngredientsApi::putToRecipe(UserId user, RecipeId recipe, IngredientId ingredient) const {
    jsonPutAuthed<void>(user, std::format("/recipes/{}/ingredients/{}", recipe, ingredient));
}

// /recipes/{recipeId}/ingredients/{ingredientId}
void IngredientsApi::deleteFromRecipe(UserId user, RecipeId recipe, IngredientId ingredient) const {
    jsonDeleteAuthed<void>(user, std::format("/recipes/{}/ingredients/{}", recipe, ingredient));
}

// /ingredients-for-recipe
IngredientSearchForRecipeResponse IngredientsApi::searchForRecipe(
    UserId user, std::string query, RecipeId recipe, std::size_t count, std::size_t offset) const {
    return jsonGetAuthed<IngredientSearchForRecipeResponse>(user,
                                                            "/ingredients-for-recipe",
                                                            {{"query", std::move(query)},
                                                             {"recipe-id", utils::to_string(recipe)},
                                                             {"size", utils::to_string(count)},
                                                             {"offset", utils::to_string(offset)}});
}

// /ingredients
IngredientId IngredientsApi::createCustom(UserId user, const IngredientCreateBody& body) const {
    return utils::parse<IngredientId>(postWithJsonAuthed(user, "/ingredients", body));
}

// /recipes/{ingredientId}/request-publication
void IngredientsApi::publishCustom(UserId user, IngredientId ingredient) const {
    jsonPostAuthed<void>(user, std::format("/ingredients/{}/request-publication", ingredient));
}

} // namespace cookcookhnya::api
