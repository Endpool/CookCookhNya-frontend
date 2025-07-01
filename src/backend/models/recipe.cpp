#include "backend/models/recipe.hpp"

#include <boost/json/conversion.hpp>
#include <boost/json/value.hpp>
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

RecipesList tag_invoke(json::value_to_tag<RecipesList> /*tag*/, const json::value& j) {
    return {
        .recipesPage = value_to<decltype(RecipesList::recipesPage)>(j.at("recipes")),
        .recipesFound = value_to<decltype(RecipesList::recipesFound)>(j.at("recipesFound")),
    };
}

void tag_invoke(json::value_from_tag /*tag*/, json::value& j, const RecipeCreateBody& body) {
    j = {{"name", body.name}};
}

RecipeCreateResponse tag_invoke(json::value_to_tag<RecipeCreateResponse> /*tag*/, const json::value& j) {
    return {.id = value_to<decltype(RecipeSummary::id)>(j.at("id"))};
}

IngredientInRecipe tag_invoke(json::value_to_tag<IngredientInRecipe> /*tag*/, const json::value& j) {
    return {
        .id = value_to<decltype(IngredientInRecipe::id)>(j.at("id")),
        .name = value_to<decltype(IngredientInRecipe::name)>(j.at("name")),
        .available = value_to<decltype(IngredientInRecipe::available)>(j.at("available")),
    };
}

RecipeDetails tag_invoke(json::value_to_tag<RecipeDetails> /*tag*/, const json::value& j) {
    return {
        .ingredients = value_to<decltype(RecipeDetails::ingredients)>(j.at("ingredients")),
        .name = value_to<decltype(RecipeDetails::name)>(j.at("name")),
        .link = value_to<decltype(RecipeDetails::link)>(j.at("link")),
    };
}

} // namespace cookcookhnya::api::models::recipe
