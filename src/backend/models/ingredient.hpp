#pragma once

#include "backend/id_types.hpp"
#include "backend/models/publication_request_status.hpp"

#include <boost/json/conversion.hpp>
#include <boost/json/value.hpp>

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

namespace cookcookhnya::api::models::ingredient {

struct Ingredient {
    IngredientId id;
    std::string name;

    friend Ingredient tag_invoke(boost::json::value_to_tag<Ingredient>, const boost::json::value& j);
};

struct CustomIngredient {
    IngredientId id;
    std::string name;
    moderation::PublicationRequestStatus moderationStatus = moderation::PublicationRequestStatus::NO_REQUEST;
    std::optional<std::string> reason = std::nullopt;

    friend CustomIngredient tag_invoke(boost::json::value_to_tag<CustomIngredient>, const boost::json::value& j);
};

struct IngredientCreateBody {
    std::string name;

    friend void tag_invoke(boost::json::value_from_tag, boost::json::value& j, const IngredientCreateBody& body);
};

struct IngredientSearchForStorageItem {
    IngredientId id;
    std::string name;
    bool isInStorage;

    friend IngredientSearchForStorageItem tag_invoke(boost::json::value_to_tag<IngredientSearchForStorageItem>,
                                                     const boost::json::value& j);
};

struct IngredientSearchForStorageResponse {
    std::vector<IngredientSearchForStorageItem> page;
    std::size_t found;

    friend IngredientSearchForStorageResponse tag_invoke(boost::json::value_to_tag<IngredientSearchForStorageResponse>,
                                                         const boost::json::value& j);
};

struct IngredientSearchForRecipeItem {
    IngredientId id;
    std::string name;
    bool isInRecipe;

    friend IngredientSearchForRecipeItem tag_invoke(boost::json::value_to_tag<IngredientSearchForRecipeItem>,
                                                    const boost::json::value& j);
};

struct IngredientSearchForRecipeResponse {
    std::vector<IngredientSearchForRecipeItem> page;
    std::size_t found;

    friend IngredientSearchForRecipeResponse tag_invoke(boost::json::value_to_tag<IngredientSearchForRecipeResponse>,
                                                        const boost::json::value& j);
};

struct IngredientSearchResponse {
    std::vector<Ingredient> page;
    std::size_t found;

    friend IngredientSearchResponse tag_invoke(boost::json::value_to_tag<IngredientSearchResponse>,
                                               const boost::json::value& j);
};

struct IngredientList {
    std::vector<Ingredient> page;
    std::size_t found;

    friend IngredientList tag_invoke(boost::json::value_to_tag<IngredientList>, const boost::json::value& j);
};

struct CustomIngredientList {
    std::vector<CustomIngredient> page;
    std::size_t found;

    friend CustomIngredientList tag_invoke(boost::json::value_to_tag<CustomIngredientList>,
                                           const boost::json::value& j);
};

} // namespace cookcookhnya::api::models::ingredient
