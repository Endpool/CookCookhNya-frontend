#pragma once

#include "backend/id_types.hpp"
#include "backend/models/storage.hpp"
#include "backend/models/user.hpp"

#include <boost/json/conversion.hpp>
#include <boost/json/value.hpp>

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace cookcookhnya::api::models::recipe {

enum class PublicationRequestStatus : std::uint8_t { Pending = 0, Accepted = 1, Rejected = 2, Idle = 3 };
PublicationRequestStatus tag_invoke(boost::json::value_to_tag<PublicationRequestStatus>, const boost::json::value& j);

struct RecipeSummary {
    RecipeId id;
    std::string name;

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
    std::vector<storage::StorageSummary> inStorages;

    friend IngredientInRecipe tag_invoke(boost::json::value_to_tag<IngredientInRecipe>, const boost::json::value& j);
};

struct RecipeDetails {
    std::vector<IngredientInRecipe> ingredients;
    std::string name;
    std::optional<std::string> link;
    std::optional<user::UserDetails> creator;
    std::optional<PublicationRequestStatus> moderationStatus;

    friend RecipeDetails tag_invoke(boost::json::value_to_tag<RecipeDetails>, const boost::json::value& j);
};

struct IngredientInCustomRecipe {
    IngredientId id;
    std::string name;

    friend IngredientInCustomRecipe tag_invoke(boost::json::value_to_tag<IngredientInCustomRecipe>,
                                               const boost::json::value& j);
};

struct RecipesList {
    std::vector<RecipeSummary> page;
    std::size_t found;

    friend RecipesList tag_invoke(boost::json::value_to_tag<RecipesList>, const boost::json::value& j);
};

struct RecipesListWithIngredientsCount {
    std::vector<RecipeSummaryWithIngredients> page;
    std::size_t found;

    friend RecipesListWithIngredientsCount tag_invoke(boost::json::value_to_tag<RecipesListWithIngredientsCount>,
                                                      const boost::json::value& j);
};

struct RecipeCreateBody {
    std::string name;
    std::vector<IngredientId> ingredients;
    std::string link;

    friend void tag_invoke(boost::json::value_from_tag, boost::json::value& j, const RecipeCreateBody& body);
};

struct RecipeSearchResponse {
    std::vector<RecipeSummary> page;
    std::size_t found;

    friend RecipeSearchResponse tag_invoke(boost::json::value_to_tag<RecipeSearchResponse>,
                                           const boost::json::value& j);
};

struct CustomRecipePublication {
    std::string created;
    std::optional<std::string> reason;
    PublicationRequestStatus status;

    friend CustomRecipePublication tag_invoke(boost::json::value_to_tag<CustomRecipePublication>,
                                              const boost::json::value& j);
};

} // namespace cookcookhnya::api::models::recipe
