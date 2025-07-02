#include "shopping_list.hpp"

#include <boost/json/conversion.hpp>
#include <boost/json/value.hpp>
#include <boost/json/value_to.hpp>

namespace cookcookhnya::api::models::shopping_list {

namespace json = boost::json;

ShoppingListItem tag_invoke(json::value_to_tag<ShoppingListItem> /*tag*/, const json::value& j) {
    return {
        .ingredientId = json::value_to<decltype(ShoppingListItem::ingredientId)>(j.at("id")),
        .name = json::value_to<decltype(ShoppingListItem::name)>(j.at("name")),
    };
}

} // namespace cookcookhnya::api::models::shopping_list
