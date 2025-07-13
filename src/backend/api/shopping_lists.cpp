#include "shopping_lists.hpp"

#include "backend/id_types.hpp"
#include "backend/models/shopping_list.hpp"

#include <httplib.h>
#include <string>
#include <vector>

namespace cookcookhnya::api {

using namespace models::shopping_list;

std::vector<ShoppingListItem> ShoppingListApi::get(UserId user) const {
    return jsonGetAuthed<std::vector<ShoppingListItem>>(user, "/my/shopping-list");
}

void ShoppingListApi::put(UserId user, const std::vector<IngredientId>& ingredients) const {
    httplib::Params params;
    for (const IngredientId id : ingredients)
        params.insert({"ingredient-id", std::to_string(id)});
    jsonPutAuthed<void>(user, "/my/shopping-list", params);
}

void ShoppingListApi::remove(UserId user, const std::vector<IngredientId>& ingredients) const {
    httplib::Params params;
    for (const IngredientId id : ingredients)
        params.insert({"ingredient-id", std::to_string(id)});
    jsonDeleteAuthed<void>(user, "/my/shopping-list", params);
}

} // namespace cookcookhnya::api
