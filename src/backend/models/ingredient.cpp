#include "backend/models/ingredient.hpp"

#include <boost/json/conversion.hpp>
#include <boost/json/value_to.hpp>

namespace cookcookhnya::api::models::ingredient {

namespace json = boost::json;

Ingredient tag_invoke(json::value_to_tag<Ingredient> /*tag*/, const json::value& j) {
    return {
        .id = value_to<decltype(Ingredient::id)>(j.at("ingredientId")),
        .name = value_to<decltype(Ingredient::name)>(j.at("name")),
    };
}

} // namespace cookcookhnya::api::models::ingredient
