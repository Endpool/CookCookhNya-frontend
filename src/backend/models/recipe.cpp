#include "backend/models/recipe.hpp"

#include <boost/json/conversion.hpp>
#include <boost/json/value.hpp>
#include <boost/json/value_from.hpp>
#include <boost/json/value_to.hpp>

namespace cookcookhnya::api::models::recipe {

namespace json = boost::json;

RecipeSummary tag_invoke(json::value_to_tag<RecipeSummary> /*tag*/, const json::value& j) {
    return {
        .id = value_to<decltype(RecipeSummary::id)>(j.at("recipeId")),
        .name = value_to<decltype(RecipeSummary::name)>(j.at("name")),
    };
}

RecipeSummaryWithIngredients tag_invoke(json::value_to_tag<RecipeSummaryWithIngredients> /*tag*/,
                                        const json::value& j) {
    return {
        .id = value_to<decltype(RecipeSummaryWithIngredients::id)>(j.at("id")),
        .name = value_to<decltype(RecipeSummaryWithIngredients::name)>(j.at("name")),
        .available = value_to<decltype(RecipeSummaryWithIngredients::available)>(j.at("available")),
        .total = value_to<decltype(RecipeSummaryWithIngredients::total)>(j.at("total")),
    };
}

RecipesListWithIngredientsCount tag_invoke(json::value_to_tag<RecipesListWithIngredientsCount> /*tag*/,
                                           const json::value& j) {
    return {
        .page = value_to<decltype(RecipesListWithIngredientsCount::page)>(j.at("results")),
        .found = value_to<decltype(RecipesListWithIngredientsCount::found)>(j.at("found")),
    };
}

void tag_invoke(json::value_from_tag /*tag*/, json::value& j, const RecipeCreateBody& body) {
    j = {{"name", body.name}, {"ingredients", boost::json::value_from(body.ingredients)}, {"sourceLink", body.link}};
}

IngredientInRecipe tag_invoke(json::value_to_tag<IngredientInRecipe> /*tag*/, const json::value& j) {
    return {
        .id = value_to<decltype(IngredientInRecipe::id)>(j.at("id")),
        .name = value_to<decltype(IngredientInRecipe::name)>(j.at("name")),
        .inStorages = value_to<decltype(IngredientInRecipe::inStorages)>(j.at("inStorages")),
    };
}

RecipeDetails tag_invoke(json::value_to_tag<RecipeDetails> /*tag*/, const json::value& j) {
    return {
        .ingredients = value_to<decltype(RecipeDetails::ingredients)>(j.at("ingredients")),
        .name = value_to<decltype(RecipeDetails::name)>(j.at("name")),
        .link = value_to<decltype(RecipeDetails::link)>(j.at("sourceLink")),
        .creator = value_to<decltype(RecipeDetails::creator)>(j.at("creator")),
    };
}

RecipeSearchResponse tag_invoke(json::value_to_tag<RecipeSearchResponse> /*tag*/, const json::value& j) {
    return {
        .page = value_to<decltype(RecipeSearchResponse::page)>(j.at("results")),
        .found = value_to<decltype(RecipeSearchResponse::found)>(j.at("found")),
    };
}
} // namespace cookcookhnya::api::models::recipe
