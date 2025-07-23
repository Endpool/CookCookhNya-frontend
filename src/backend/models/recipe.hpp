#pragma once

#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"
#include "publication_request_status.hpp"
#include "storage.hpp"
#include "user.hpp"

#include <boost/json/conversion.hpp>
#include <boost/json/value.hpp>

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

namespace cookcookhnya::api::models::recipe {

struct RecipeSummary {
    RecipeId id;
    std::string name;

    friend RecipeSummary tag_invoke(boost::json::value_to_tag<RecipeSummary>, const boost::json::value& j);
};

struct RecipeDetails {
    std::vector<ingredient::Ingredient> ingredients;
    std::string name;
    std::optional<std::string> link;
    std::optional<user::UserDetails> creator;

    friend RecipeDetails tag_invoke(boost::json::value_to_tag<RecipeDetails>, const boost::json::value& j);
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

struct SuggestedRecipeDetails {
    std::vector<IngredientInRecipe> ingredients;
    std::string name;
    std::optional<std::string> link;
    std::optional<user::UserDetails> creator;
    moderation::PublicationRequestStatus moderationStatus = moderation::PublicationRequestStatus::NO_REQUEST;

    friend SuggestedRecipeDetails tag_invoke(boost::json::value_to_tag<SuggestedRecipeDetails>,
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

struct RecipePublicationRequest {
    moderation::PublicationRequestStatus status = moderation::PublicationRequestStatus::NO_REQUEST;
    std::chrono::system_clock::time_point created;
    std::optional<std::chrono::system_clock::time_point> updated;
    std::optional<std::string> reason;

    friend RecipePublicationRequest tag_invoke(boost::json::value_to_tag<RecipePublicationRequest>,
                                               const boost::json::value& j);
};

} // namespace cookcookhnya::api::models::recipe
