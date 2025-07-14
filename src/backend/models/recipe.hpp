#pragma once

#include "backend/id_types.hpp"
#include "tg_types.hpp"

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

struct RecipeCreator {
    tg_types::UserId id;
    std::string fullName;

    friend RecipeCreator tag_invoke(boost::json::value_to_tag<RecipeCreator>, const boost::json::value& j);
};

struct RecipeDetails {
    std::vector<IngredientInRecipe> ingredients;
    std::string name; // RECIPE NAME
    std::string link;
    RecipeCreator creator;

    friend RecipeDetails tag_invoke(boost::json::value_to_tag<RecipeDetails>, const boost::json::value& j);
};

struct IngredientInCustomRecipe {
    IngredientId id;
    std::string name;

    friend IngredientInCustomRecipe tag_invoke(boost::json::value_to_tag<IngredientInCustomRecipe>,
                                               const boost::json::value& j);
};

struct CustomRecipeDetails { // Exists because of field inStorages which is not needed here
    std::vector<IngredientInCustomRecipe> ingredients;
    std::string name; // RECIPE NAME
    std::string link;

    friend CustomRecipeDetails tag_invoke(boost::json::value_to_tag<CustomRecipeDetails>, const boost::json::value& j);
};

struct RecipesList {
    std::vector<RecipeSummary> recipesPage;
    int recipesFound;

    friend RecipesList tag_invoke(boost::json::value_to_tag<RecipesList>, const boost::json::value& j);
};

struct CustomRecipeSummary {
    RecipeId id;
    std::string name;
    std::string link;

    friend CustomRecipeSummary tag_invoke(boost::json::value_to_tag<CustomRecipeSummary>, const boost::json::value& j);
};

struct CustomRecipesList {
    std::vector<CustomRecipeSummary> recipesPage;
    int recipesFound;

    friend CustomRecipesList tag_invoke(boost::json::value_to_tag<CustomRecipesList>, const boost::json::value& j);
};
struct RecipeCreateBody {
    std::string name;
    std::vector<IngredientId> ingredients;
    std::string link;

    friend void tag_invoke(boost::json::value_from_tag /*tag*/, boost::json::value& j, const RecipeCreateBody& body);
};

} // namespace cookcookhnya::api::models::recipe
