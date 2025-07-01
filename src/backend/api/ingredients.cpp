#include "backend/api/ingredients.hpp"

#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"

#include <format>
#include <ranges>
#include <string>
#include <unordered_set>
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

std::vector<IngredientSearchResult> IngredientsApi::search(UserId user, std::string query, StorageId storage) const {
    // return jsonGetAuthed<std::vector<IngredientSearchResult>>(
    //     user, "/ingredients-for-storage", {{"query", std::move(query)}, {"storage", std::to_string(storage)}});
    using namespace std::views;
    using std::ranges::to;
    if (!query.empty() && query[0] == '\0') // stub
        query[0] = 'a';
    auto sis = getStorageIngredients(user, storage) | transform(&Ingredient::id) | to<std::unordered_set>();
    return getAllIngredients() | transform([&sis](Ingredient& i) {
               return IngredientSearchResult{.id = i.id, .name = std::move(i.name), .available = sis.contains(i.id)};
           }) |
           take(query.size()) | to<std::vector>();
}

} // namespace cookcookhnya::api
