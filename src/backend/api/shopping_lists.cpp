#include "shopping_lists.hpp"

#include "backend/id_types.hpp"
#include "backend/models/shopping_list.hpp"

#include <httplib.h>
#include <string>
#include <vector>

namespace cookcookhnya::api {

using namespace models::shopping_list;

std::vector<ShoppingListItem> ShoppingListApi::get(UserId userId) const {
    return jsonGetAuthed<std::vector<ShoppingListItem>>(userId, "/my/shopping-list");
}

void ShoppingListApi::put(UserId userId, const std::vector<IngredientId>& ingredientIds) const {
    httplib::Params params;
    for (const IngredientId id : ingredientIds)
        params.insert({"ingredient-id", std::to_string(id)});
    jsonPutAuthed<void>(userId, "/my/shopping-list", params);
}

void ShoppingListApi::remove(UserId userId, const std::vector<IngredientId>& ingredientIds) const {
    httplib::Params params;
    for (const IngredientId id : ingredientIds)
        params.insert({"ingredient-id", std::to_string(id)});
    jsonDeleteAuthed<void>(userId, "/my/shopping-list", params);
}

} // namespace cookcookhnya::api
