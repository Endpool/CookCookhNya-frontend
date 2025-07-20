#include "backend/models/recipe.hpp"
#include "utils/serialization.hpp"

#include <boost/json/conversion.hpp>
#include <boost/json/value.hpp>
#include <boost/json/value_from.hpp>
#include <boost/json/value_to.hpp>
#include <termios.h>

namespace cookcookhnya::api::models::recipe {

namespace json = boost::json;

RecipeSummary tag_invoke(json::value_to_tag<RecipeSummary> /*tag*/, const json::value& j) {
    return {
        .id = value_to<decltype(RecipeSummary::id)>(j.at("recipeId")),
        .name = value_to<decltype(RecipeSummary::name)>(j.at("name")),
    };
}

RecipeSummaryWithIngredients tag_invoke(json::value_to_tag<RecipeSummaryWithIngredients> /*tag*/,
                                        const json::value& j) {
    return {
        .id = value_to<decltype(RecipeSummaryWithIngredients::id)>(j.at("id")),
        .name = value_to<decltype(RecipeSummaryWithIngredients::name)>(j.at("name")),
        .available = value_to<decltype(RecipeSummaryWithIngredients::available)>(j.at("available")),
        .total = value_to<decltype(RecipeSummaryWithIngredients::total)>(j.at("total")),
    };
}

RecipesListWithIngredientsCount tag_invoke(json::value_to_tag<RecipesListWithIngredientsCount> /*tag*/,
                                           const json::value& j) {
    return {
        .page = value_to<decltype(RecipesListWithIngredientsCount::page)>(j.at("results")),
        .found = value_to<decltype(RecipesListWithIngredientsCount::found)>(j.at("found")),
    };
}

void tag_invoke(json::value_from_tag /*tag*/, json::value& j, const RecipeCreateBody& body) {
    j = {{"name", body.name}, {"ingredients", boost::json::value_from(body.ingredients)}, {"sourceLink", body.link}};
}

IngredientInRecipe tag_invoke(json::value_to_tag<IngredientInRecipe> /*tag*/, const json::value& j) {
    return {
        .id = value_to<decltype(IngredientInRecipe::id)>(j.at("id")),
        .name = value_to<decltype(IngredientInRecipe::name)>(j.at("name")),
        .inStorages = value_to<decltype(IngredientInRecipe::inStorages)>(j.at("inStorages")),
    };
}

RecipeDetails tag_invoke(json::value_to_tag<RecipeDetails> /*tag*/, const json::value& j) {
    return {
        .ingredients = value_to<decltype(RecipeDetails::ingredients)>(j.at("ingredients")),
        .name = value_to<decltype(RecipeDetails::name)>(j.at("name")),
        .link = value_to<decltype(RecipeDetails::link)>(j.at("sourceLink")),
        // Deal with optionals using ternary
        .creator = j.as_object().if_contains("creator") ? value_to<decltype(RecipeDetails::creator)>(j.at("creator"))
                                                        : user::UserDetails{.userId = 0, .alias = "", .fullName = ""},
        .moderationStatus = j.as_object().if_contains("status")
                                ? value_to<decltype(RecipeDetails::moderationStatus)>(j.at("status"))
                                : PublicationRequestStatus::NO_REQUEST,
    };
}

IngredientInCustomRecipe tag_invoke(json::value_to_tag<IngredientInCustomRecipe> /*tag*/, const json::value& j) {
    return {
        .id = value_to<decltype(IngredientInCustomRecipe::id)>(j.at("id")),
        .name = value_to<decltype(IngredientInCustomRecipe::name)>(j.at("name")),
    };
}

RecipeSearchResponse tag_invoke(json::value_to_tag<RecipeSearchResponse> /*tag*/, const json::value& j) {
    return {
        .page = value_to<decltype(RecipeSearchResponse::page)>(j.at("results")),
        .found = value_to<decltype(RecipeSearchResponse::found)>(j.at("found")),
    };
}

PublicationHistoryRecipe tag_invoke(json::value_to_tag<PublicationHistoryRecipe> /*tag*/, const json::value& j) {
    return {
        .created = utils::parseIsoTime(value_to<std::string>(j.at("created"))),
        .reason = j.as_object().if_contains("reason")
                      ? value_to<decltype(PublicationHistoryRecipe::reason)>(j.at("reason"))
                      : "",
        .status = j.as_object().if_contains("status")
                      ? value_to<decltype(PublicationHistoryRecipe::status)>(j.at("status"))
                      : PublicationRequestStatus::NO_REQUEST,
        .updated = j.as_object().if_contains("updated") ? utils::parseIsoTime(value_to<std::string>(j.at("updated")))
                                                        : std::chrono::time_point<std::chrono::system_clock>(),
    };
}

PublicationRequestStatus tag_invoke(boost::json::value_to_tag<PublicationRequestStatus> /*tag*/,
                                    const boost::json::value& j) {
    if (j.at("status") == "Pending")
        return PublicationRequestStatus::PENDING;
    if (j.at("status") == "Accepted")
        return PublicationRequestStatus::ACCEPTED;
    if (j.at("status") == "Rejected")
        return PublicationRequestStatus::REJECTED;
    return PublicationRequestStatus::NO_REQUEST;
}
} // namespace cookcookhnya::api::models::recipe
