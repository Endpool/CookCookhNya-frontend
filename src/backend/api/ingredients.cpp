#include "backend/api/ingredients.hpp"

#include "backend/models/ingredient.hpp"

#include <format>
#include <vector>

namespace cookcookhnya::api {

using namespace models::ingredient;

std::vector<Ingredient> IngredientsApi::getStorageIngredients(UserId user, StorageId storage) const {
    return jsonGetAuthed<std::vector<Ingredient>>(user, std::format("/my/storages/{}/ingredients", storage));
}

void IngredientsApi::put(UserId user, StorageId storage, IngredientId id) const {
    jsonPutAuthed<void>(user, std::format("/my/storages/{}/ingredients/{}", storage, id));
}

void IngredientsApi::delete_(UserId user, StorageId storage, IngredientId id) const {
    jsonDeleteAuthed<void>(user, std::format("/my/storages/{}/ingredients/{}", storage, id));
}

} // namespace cookcookhnya::api
