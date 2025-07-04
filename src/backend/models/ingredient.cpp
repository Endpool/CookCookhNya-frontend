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

IngredientSearchItem tag_invoke(json::value_to_tag<IngredientSearchItem> /*tag*/, const json::value& j) {
    return {
        .id = value_to<decltype(IngredientSearchItem::id)>(j.at("id")),
        .name = value_to<decltype(IngredientSearchItem::name)>(j.at("name")),
        .available = value_to<decltype(IngredientSearchItem::available)>(j.at("available")),
    };
}

IngredientSearchResponse tag_invoke(json::value_to_tag<IngredientSearchResponse> /*tag*/, const json::value& j) {
    return {
        .page = value_to<decltype(IngredientSearchResponse::page)>(j.at("results")),
        .found = value_to<decltype(IngredientSearchResponse::found)>(j.at("found")),
    };
}

} // namespace cookcookhnya::api::models::ingredient
