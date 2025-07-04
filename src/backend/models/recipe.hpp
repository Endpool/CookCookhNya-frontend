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
    std::vector<StorageId> inStorages;

    friend IngredientInRecipe tag_invoke(boost::json::value_to_tag<IngredientInRecipe>, const boost::json::value& j);
};

struct RecipeDetails {
    std::vector<IngredientInRecipe> ingredients;
    std::string name; // RECIPE NAME
    std::string link;

    friend RecipeDetails tag_invoke(boost::json::value_to_tag<RecipeDetails>, const boost::json::value& j);
};

struct RecipesList {
    std::vector<RecipeSummary> recipesPage;
    int recipesFound;

    friend RecipesList tag_invoke(boost::json::value_to_tag<RecipesList>, const boost::json::value& j);
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
