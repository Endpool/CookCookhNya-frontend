#include "backend/api/ingredients.hpp"

#include "backend/models/ingredient.hpp"

#include <format>
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

std::vector<IngredientId> IngredientsApi::getAllIngredients() const {
    return jsonGet<std::vector<IngredientId>>("/ingredients");
    // return jsonGet<std::vector<Ingredient>>("/ingredients");
}

Ingredient IngredientsApi::get(IngredientId id) const {
    return jsonGet<Ingredient>(std::format("/ingredients/{}", id));
    // return jsonGet<std::vector<Ingredient>>("/ingredients");
}

} // namespace cookcookhnya::api
