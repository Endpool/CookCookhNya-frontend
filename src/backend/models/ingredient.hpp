#pragma once

#include "backend/id_types.hpp"

#include <boost/json/conversion.hpp>
#include <boost/json/value.hpp>

#include <cstddef>
#include <string>
#include <vector>

namespace cookcookhnya::api::models::ingredient {

struct Ingredient {
    IngredientId id;
    std::string name;

    friend Ingredient tag_invoke(boost::json::value_to_tag<Ingredient>, const boost::json::value& j);
};

struct IngredientSearchItem {
    IngredientId id;
    std::string name;
    bool available;

    friend IngredientSearchItem tag_invoke(boost::json::value_to_tag<IngredientSearchItem>,
                                           const boost::json::value& j);
};

struct IngredientSearchResponse {
    std::vector<IngredientSearchItem> page;
    std::size_t found;

    friend IngredientSearchResponse tag_invoke(boost::json::value_to_tag<IngredientSearchResponse>,
                                               const boost::json::value& j);
};

} // namespace cookcookhnya::api::models::ingredient
