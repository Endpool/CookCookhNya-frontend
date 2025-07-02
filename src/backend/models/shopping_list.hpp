#pragma once

#include "backend/id_types.hpp"

#include <boost/json/conversion.hpp>
#include <boost/json/value.hpp>

#include <string>

namespace cookcookhnya::api::models::shopping_list {

struct ShoppingListItem {
    IngredientId ingredientId;
    std::string name;

    friend ShoppingListItem tag_invoke(boost::json::value_to_tag<ShoppingListItem>, const boost::json::value& j);
};

} // namespace cookcookhnya::api::models::shopping_list
