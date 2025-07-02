#pragma once

#include "backend/id_types.hpp"

#include <boost/json/conversion.hpp>
#include <boost/json/value.hpp>

#include <string>

namespace cookcookhnya::api::models::ingredient {

struct Ingredient {
    IngredientId id;
    std::string name;

    friend Ingredient tag_invoke(boost::json::value_to_tag<Ingredient>, const boost::json::value& j);
};

struct IngredientSearchResult {
    IngredientId id;
    std::string name;
    bool available;

    friend IngredientSearchResult tag_invoke(boost::json::value_to_tag<IngredientSearchResult>,
                                             const boost::json::value& j);
};

} // namespace cookcookhnya::api::models::ingredient
