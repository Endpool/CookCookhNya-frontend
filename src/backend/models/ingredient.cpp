#include "backend/models/ingredient.hpp"

#include <boost/json/conversion.hpp>
#include <boost/json/value.hpp>
#include <boost/json/value_to.hpp>

namespace cookcookhnya::api::models::ingredient {

namespace json = boost::json;

Ingredient tag_invoke(json::value_to_tag<Ingredient> /*tag*/, const json::value& j) {
    return {
        .id = value_to<decltype(Ingredient::id)>(j.at("id")),
        .name = value_to<decltype(Ingredient::name)>(j.at("name")),
    };
}

IngredientSearchResult tag_invoke(json::value_to_tag<IngredientSearchResult> /*tag*/, const json::value& j) {
    return {
        .id = value_to<decltype(IngredientSearchResult::id)>(j.at("id")),
        .name = value_to<decltype(IngredientSearchResult::name)>(j.at("name")),
        .available = value_to<decltype(IngredientSearchResult::available)>(j.at("available")),
    };
}

} // namespace cookcookhnya::api::models::ingredient
