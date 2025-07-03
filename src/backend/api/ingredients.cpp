#include "backend/api/ingredients.hpp"

#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"

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

std::vector<Ingredient> IngredientsApi::getAllIngredients() const {
    return jsonGet<std::vector<Ingredient>>("/ingredients");
}

Ingredient IngredientsApi::get(IngredientId id) const {
    return jsonGet<Ingredient>(std::format("/ingredients/{}", id));
}

IngredientSearchResponse IngredientsApi::search(
    UserId userId, std::string query, StorageId storage, std::size_t count, std::size_t offset) const {
    return jsonGetAuthed<IngredientSearchResponse>(userId,
                                                   "/ingredients-for-storage",
                                                   {{"query", std::move(query)},
                                                    {"storage-id", std::to_string(storage)},
                                                    {"size", std::to_string(count)},
                                                    {"offset", std::to_string(offset)}});
}

} // namespace cookcookhnya::api
