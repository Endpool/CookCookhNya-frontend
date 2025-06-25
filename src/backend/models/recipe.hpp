#pragma once

#include "backend/id_types.hpp"

#include <boost/json/conversion.hpp>
#include <boost/json/value.hpp>

#include <string>
#include <vector>

namespace cookcookhnya::api::models::recipe {

struct RecipeSummary {
    RecipeId id;
    std::string name;
    int available;
    int total;

    friend RecipeSummary tag_invoke(boost::json::value_to_tag<RecipeSummary>, const boost::json::value& j);
};

struct IngredientInRecipe {
    IngredientId id;
    std::string name;
    bool available;

    friend IngredientInRecipe tag_invoke(boost::json::value_to_tag<IngredientInRecipe>, const boost::json::value& j);
};

struct IngredientsInRecipe {
    std::vector<IngredientInRecipe> ingredients;
    std::string name; // RECIPE NAME
    std::string link;

    friend IngredientsInRecipe tag_invoke(boost::json::value_to_tag<IngredientsInRecipe>, const boost::json::value& j);
};

struct RecipesSummary {
    std::vector<RecipeSummary> recipes;
    int recipesFound;

    friend RecipesSummary tag_invoke(boost::json::value_to_tag<RecipesSummary>, const boost::json::value& j);
};

struct RecipeCreateBody {
    std::string name;

    friend void tag_invoke(boost::json::value_from_tag, boost::json::value& j, const RecipeCreateBody& body);
};

struct RecipeCreateResponse {
    RecipeId id;

    friend RecipeCreateResponse tag_invoke(boost::json::value_to_tag<RecipeCreateResponse>,
                                           const boost::json::value& j);
};

} // namespace cookcookhnya::api::models::recipe
