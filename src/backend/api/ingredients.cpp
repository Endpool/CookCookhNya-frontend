#include "backend/api/ingredients.hpp"

#include "backend/api/publicity_filter.hpp"
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

// GET /ingredients/{ingredientId}
Ingredient IngredientsApi::get(UserId user, IngredientId ingredient) const {
    return jsonGetAuthed<Ingredient>(user, std::format("/ingredients/{}", ingredient));
}

// GET /storages/{storageId}/ingredients
std::vector<Ingredient>
IngredientsApi::getStorageIngredients(UserId user, StorageId storage, std::size_t count, std::size_t offset) const {
    return jsonGetAuthed<std::vector<Ingredient>>(
        user,
        std::format("/storages/{}/ingredients", storage),
        {{"size", utils::to_string(count)}, {"offset", utils::to_string(offset)}});
}

// PUT /storages/{storageId}/ingredients/{ingredientId}
void IngredientsApi::putToStorage(UserId user, StorageId storage, IngredientId ingredient) const {
    jsonPutAuthed<void>(user, std::format("/storages/{}/ingredients/{}", storage, ingredient));
}

// DELETE /storages/{storageId}/ingredients/{ingredientId}
void IngredientsApi::deleteFromStorage(UserId user, StorageId storage, IngredientId ingredient) const {
    jsonDeleteAuthed<void>(user, std::format("/storages/{}/ingredients/{}", storage, ingredient));
}

// DELETE /storages/{storageId}/ingredients
void IngredientsApi::deleteMultipleFromStorage(UserId user,
                                               StorageId storage,
                                               const std::vector<IngredientId>& ingredients) const {
    httplib::Params params = {};
    for (auto id : ingredients)
        params.emplace("ingredient", utils::to_string(id));
    jsonDeleteAuthed<void>(user, std::format("/storages/{}/ingredients/", storage), params);
}

// GET /ingredients-for-storage
IngredientSearchForStorageResponse IngredientsApi::searchForStorage(UserId user,
                                                                    StorageId storage,
                                                                    std::string query,
                                                                    std::size_t count,
                                                                    std::size_t offset,
                                                                    unsigned threshold) const {
    return jsonGetAuthed<IngredientSearchForStorageResponse>(user,
                                                             "/ingredients-for-storage",
                                                             {{"query", std::move(query)},
                                                              {"storage-id", utils::to_string(storage)},
                                                              {"size", utils::to_string(count)},
                                                              {"threshold", utils::to_string(threshold)},
                                                              {"offset", utils::to_string(offset)}});
}

// GET /ingredients
IngredientSearchResponse IngredientsApi::search(UserId user,
                                                PublicityFilterType filter,
                                                std::string query,
                                                std::size_t count,
                                                std::size_t offset,
                                                unsigned threshold) const {
    return jsonGetAuthed<IngredientSearchResponse>(user,
                                                   "/ingredients",
                                                   {{"query", std::move(query)},
                                                    {"size", utils::to_string(count)},
                                                    {"offset", utils::to_string(offset)},
                                                    {"threshold", utils::to_string(threshold)},
                                                    {"filter", utils::to_string(filter)}});
}

// GET /recipes
IngredientList
IngredientsApi::getList(UserId user, PublicityFilterType filter, std::size_t count, std::size_t offset) const {
    auto result = search(user, filter, "", count, offset, 0);
    return {.page = std::move(result.page), .found = result.found};
}

// GET /public/ingredients/{ingredientId}
Ingredient IngredientsApi::getPublicIngredient(IngredientId ingredient) const {
    return jsonGet<Ingredient>(std::format("/public/ingredients/{}", ingredient));
}

// PUT /recipes/{recipeId}/ingredients/{ingredientId}
void IngredientsApi::putToRecipe(UserId user, RecipeId recipe, IngredientId ingredient) const {
    jsonPutAuthed<void>(user, std::format("/recipes/{}/ingredients/{}", recipe, ingredient));
}

// DELETE /recipes/{recipeId}/ingredients/{ingredientId}
void IngredientsApi::deleteFromRecipe(UserId user, RecipeId recipe, IngredientId ingredient) const {
    jsonDeleteAuthed<void>(user, std::format("/recipes/{}/ingredients/{}", recipe, ingredient));
}

// GET /ingredients-for-recipe
IngredientSearchForRecipeResponse IngredientsApi::searchForRecipe(
    UserId user, RecipeId recipe, std::string query, std::size_t count, std::size_t offset, unsigned threshold) const {
    return jsonGetAuthed<IngredientSearchForRecipeResponse>(user,
                                                            "/ingredients-for-recipe",
                                                            {{"query", std::move(query)},
                                                             {"threshold", utils::to_string(threshold)},
                                                             {"size", utils::to_string(count)},
                                                             {"offset", utils::to_string(offset)},
                                                             {"recipe-id", utils::to_string(recipe)}});
}

// POST /ingredients
IngredientId IngredientsApi::createCustom(UserId user, const IngredientCreateBody& body) const {
    return utils::parse<IngredientId>(postWithJsonAuthed(user, "/ingredients", body));
}

// POST /recipes/{ingredientId}/request-publication
void IngredientsApi::publishCustom(UserId user, IngredientId ingredient) const {
    jsonPostAuthed<void>(user, std::format("/ingredients/{}/request-publication", ingredient));
}

} // namespace cookcookhnya::api
