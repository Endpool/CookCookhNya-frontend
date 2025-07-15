#pragma once

#include "backend/id_types.hpp"
#include "backend/models/user.hpp"

#include <boost/json/conversion.hpp>
#include <boost/json/value.hpp>

#include <optional>
#include <string>
#include <vector>

namespace cookcookhnya::api::models::recipe {

struct RecipeSummary {
    RecipeId id;
    std::string name;
    std::string sourceLink;

    friend RecipeSummary tag_invoke(boost::json::value_to_tag<RecipeSummary>, const boost::json::value& j);
};

struct RecipeSummaryWithIngredients {
    RecipeId id;
    std::string name;
    std::size_t available;
    std::size_t total;

    friend RecipeSummaryWithIngredients tag_invoke(boost::json::value_to_tag<RecipeSummaryWithIngredients>,
                                                   const boost::json::value& j);
};

struct IngredientInRecipe {
    IngredientId id;
    std::string name;
    std::vector<StorageId> inStorages;

    friend IngredientInRecipe tag_invoke(boost::json::value_to_tag<IngredientInRecipe>, const boost::json::value& j);
};

struct RecipeDetails {
    std::vector<IngredientInRecipe> ingredients;
    std::string name;
    std::string link;
    user::UserDetails creator;

    friend RecipeDetails tag_invoke(boost::json::value_to_tag<RecipeDetails>, const boost::json::value& j);
};

struct IngredientInCustomRecipe {
    IngredientId id;
    std::string name;

    friend IngredientInCustomRecipe tag_invoke(boost::json::value_to_tag<IngredientInCustomRecipe>,
                                               const boost::json::value& j);
};

struct CustomRecipeDetails {
    std::vector<IngredientInCustomRecipe> ingredients;
    std::string name;
    std::string link;

    friend CustomRecipeDetails tag_invoke(boost::json::value_to_tag<CustomRecipeDetails>, const boost::json::value& j);
};

struct RecipesList {
    std::vector<RecipeSummary> page;
    std::size_t found;

    friend RecipesList tag_invoke(boost::json::value_to_tag<RecipesList>, const boost::json::value& j);
};

struct CustomRecipeSummary {
    RecipeId id;
    std::string name;

    friend CustomRecipeSummary tag_invoke(boost::json::value_to_tag<CustomRecipeSummary>, const boost::json::value& j);
};

struct CustomRecipesList {
    std::vector<CustomRecipeSummary> page;
    std::size_t found;

    friend CustomRecipesList tag_invoke(boost::json::value_to_tag<CustomRecipesList>, const boost::json::value& j);
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

struct RecipeSearchResponse {
    std::vector<RecipeSummary> page;
    std::size_t found;

    friend RecipeSearchResponse tag_invoke(boost::json::value_to_tag<RecipeSearchResponse>,
                                           const boost::json::value& j);
};

} // namespace cookcookhnya::api::models::recipe
