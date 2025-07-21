#include "backend/models/ingredient.hpp"
#include "backend/models/publication_request_status.hpp"

#include <boost/json/conversion.hpp>
#include <boost/json/value.hpp>
#include <boost/json/value_to.hpp>

namespace cookcookhnya::api::models::ingredient {

namespace json = boost::json;

Ingredient tag_invoke(json::value_to_tag<Ingredient> /*tag*/, const json::value& j) {
    return {
        .id = value_to<decltype(Ingredient::id)>(j.at("id")),
        .name = value_to<decltype(Ingredient::name)>(j.at("name")),
        .moderationStatus = j.as_object().if_contains("moderationStatus")
                                ? value_to<decltype(Ingredient::moderationStatus)>(j.at("moderationStatus"))
                                : moderation::PublicationRequestStatus::NO_REQUEST,
    };
}

void tag_invoke(json::value_from_tag /*tag*/, json::value& j, const IngredientCreateBody& body) {
    j = {{"name", body.name}};
}

IngredientSearchForStorageItem tag_invoke(json::value_to_tag<IngredientSearchForStorageItem> /*tag*/,
                                          const json::value& j) {
    return {
        .id = value_to<decltype(IngredientSearchForStorageItem::id)>(j.at("id")),
        .name = value_to<decltype(IngredientSearchForStorageItem::name)>(j.at("name")),
        .isInStorage = value_to<decltype(IngredientSearchForStorageItem::isInStorage)>(j.at("isInStorage")),
    };
}

IngredientSearchForStorageResponse tag_invoke(json::value_to_tag<IngredientSearchForStorageResponse> /*tag*/,
                                              const json::value& j) {
    return {
        .page = value_to<decltype(IngredientSearchForStorageResponse::page)>(j.at("results")),
        .found = value_to<decltype(IngredientSearchForStorageResponse::found)>(j.at("found")),
    };
}

IngredientSearchForRecipeItem tag_invoke(json::value_to_tag<IngredientSearchForRecipeItem> /*tag*/,
                                         const json::value& j) {
    return {
        .id = value_to<decltype(IngredientSearchForRecipeItem::id)>(j.at("id")),
        .name = value_to<decltype(IngredientSearchForRecipeItem::name)>(j.at("name")),
        .isInRecipe = value_to<decltype(IngredientSearchForRecipeItem::isInRecipe)>(j.at("isInRecipe")),
    };
}

IngredientSearchForRecipeResponse tag_invoke(json::value_to_tag<IngredientSearchForRecipeResponse> /*tag*/,
                                             const json::value& j) {
    return {
        .page = value_to<decltype(IngredientSearchForRecipeResponse::page)>(j.at("results")),
        .found = value_to<decltype(IngredientSearchForRecipeResponse::found)>(j.at("found")),
    };
}

IngredientSearchResponse tag_invoke(json::value_to_tag<IngredientSearchResponse> /*tag*/, const json::value& j) {
    return {
        .page = value_to<decltype(IngredientSearchResponse::page)>(j.at("results")),
        .found = value_to<decltype(IngredientSearchResponse::found)>(j.at("found")),
    };
}

IngredientList tag_invoke(json::value_to_tag<IngredientList> /*tag*/, const json::value& j) {
    return {
        .page = value_to<decltype(IngredientList::page)>(j.at("results")),
        .found = value_to<decltype(IngredientList::found)>(j.at("found")),
    };
}

} // namespace cookcookhnya::api::models::ingredient
