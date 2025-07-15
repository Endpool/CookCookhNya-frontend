#include "backend/models/recipe.hpp"

#include <boost/json/conversion.hpp>
#include <boost/json/value.hpp>
#include <boost/json/value_from.hpp>
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
        .recipesFound = value_to<decltype(RecipesList::recipesFound)>(j.at("found")),
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

RecipeCreator tag_invoke(json::value_to_tag<RecipeCreator> /*tag*/, const json::value& j) {
    return {
        .id = value_to<decltype(RecipeCreator::id)>(j.at("id")),
        .fullName = value_to<decltype(RecipeCreator::fullName)>(j.at("fullName")),

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

CustomRecipeSummary tag_invoke(json::value_to_tag<CustomRecipeSummary> /*tag*/, const json::value& j) {
    return {
        .id = value_to<decltype(CustomRecipeSummary::id)>(j.at("recipeId")),
        .name = value_to<decltype(CustomRecipeSummary::name)>(j.at("name")),
        .link = value_to<decltype(CustomRecipeSummary::link)>(j.at("sourceLink")),
    };
}

CustomRecipesList tag_invoke(json::value_to_tag<CustomRecipesList> /*tag*/, const json::value& j) {
    return {
        .recipesPage = value_to<decltype(CustomRecipesList::recipesPage)>(j.at("recipes")),
        .recipesFound = value_to<decltype(CustomRecipesList::recipesFound)>(j.at("found")),
    };
}

CustomRecipeDetails tag_invoke(json::value_to_tag<CustomRecipeDetails> /*tag*/, const json::value& j) {
    return {
        .ingredients = value_to<decltype(CustomRecipeDetails::ingredients)>(j.at("ingredients")),
        .name = value_to<decltype(CustomRecipeDetails::name)>(j.at("name")),
        .link = value_to<decltype(CustomRecipeDetails::link)>(j.at("sourceLink")),
    };
}

IngredientInCustomRecipe tag_invoke(json::value_to_tag<IngredientInCustomRecipe> /*tag*/, const json::value& j) {
    return {
        .id = value_to<decltype(IngredientInCustomRecipe::id)>(j.at("id")),
        .name = value_to<decltype(IngredientInCustomRecipe::name)>(j.at("name")),
    };
}

} // namespace cookcookhnya::api::models::recipe
