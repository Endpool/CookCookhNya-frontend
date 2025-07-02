#include "shopping_lists.hpp"

#include "backend/models/shopping_list.hpp"

#include <vector>

namespace cookcookhnya::api {

using namespace models::shopping_list;

std::vector<ShoppingListItem> ShoppingListApi::get(UserId userId) const {
    return jsonGetAuthed<std::vector<ShoppingListItem>>(userId, "/my/shopping-list");
}

} // namespace cookcookhnya::api
