#include "backend/models/recipe.hpp"

#include <boost/json/conversion.hpp>
#include <boost/json/value_to.hpp>

namespace cookcookhnya::api::models::recipe {

namespace json = boost::json;

RecipeSummary tag_invoke(json::value_to_tag<RecipeSummary> /*tag*/, const json::value& j) {
    return {
        .id = value_to<decltype(RecipeSummary::id)>(j.at("id")),
        .name = value_to<decltype(RecipeSummary::name)>(j.at("name")),
        .available = value_to<decltype(RecipeSummary::available)>(j.at("available")),
        .total = value_to<decltype(RecipeSummary::total)>(j.at("total")),
    };
}

RecipesSummary tag_invoke(json::value_to_tag<RecipesSummary> /*tag*/, const json::value& j) {
    return {
        .recipes = value_to<decltype(RecipesSummary::recipes)>(j.at("recipes")),
        .recipesFound = value_to<decltype(RecipesSummary::recipesFound)>(j.at("recipesFound")),
    };
}

void tag_invoke(json::value_from_tag /*tag*/, json::value& j, const RecipeCreateBody& body) {
    j = {{"name", body.name}};
}

RecipeCreateResponse tag_invoke(json::value_to_tag<RecipeCreateResponse> /*tag*/, const json::value& j) {
    return {.id = value_to<decltype(RecipeSummary::id)>(j.at("id"))};
}

} // namespace cookcookhnya::api::models::recipe
