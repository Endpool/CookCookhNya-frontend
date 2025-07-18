#include "shopping_lists.hpp"

#include "backend/id_types.hpp"
#include "backend/models/shopping_list.hpp"
#include "utils/to_string.hpp"

#include <cstddef>
#include <httplib.h>
#include <string>
#include <vector>

namespace cookcookhnya::api {

using namespace models::shopping_list;

// GET /shopping-list
std::vector<ShoppingListItem> ShoppingListApi::get(UserId user, std::size_t count, std::size_t offset) const {
    return jsonGetAuthed<std::vector<ShoppingListItem>>(
        user, "/shopping-list", {{"size", utils::to_string(count)}, {"offset", utils::to_string(offset)}});
}

// PUT /shopping-list
void ShoppingListApi::put(UserId user, const std::vector<IngredientId>& ingredients) const {
    httplib::Params params;
    for (const IngredientId id : ingredients)
        params.emplace("ingredient-id", utils::to_string(id));
    jsonPutAuthed<void>(user, "/shopping-list", params);
}

// DELETE /shopping-list
void ShoppingListApi::remove(UserId user, const std::vector<IngredientId>& ingredients) const {
    httplib::Params params;
    for (const IngredientId id : ingredients)
        params.emplace("ingredient-id", utils::to_string(id));
    jsonDeleteAuthed<void>(user, "/shopping-list", params);
}

// PUT /shopping-list/buy
void ShoppingListApi::buy(UserId user, StorageId storage, const std::vector<IngredientId>& ingredients) const {
    httplib::Params params;
    for (const IngredientId id : ingredients)
        params.emplace("ingredient-id", utils::to_string(id));
    params.emplace("storage-id", utils::to_string(storage));
    jsonPutAuthed<void>(user, "/shopping-list/buy", params);
}

} // namespace cookcookhnya::api
