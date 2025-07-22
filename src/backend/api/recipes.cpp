#include "backend/api/recipes.hpp"

#include "backend/api/publicity_filter.hpp"
#include "backend/id_types.hpp"
#include "backend/models/recipe.hpp"
#include "utils/parsing.hpp"
#include "utils/to_string.hpp"

#include <httplib.h>

#include <cstddef>
#include <format>
#include <string>
#include <utility>
#include <vector>

namespace cookcookhnya::api {

using namespace models::recipe;

// GET /suggested-recipes
RecipesListWithIngredientsCount RecipesApi::getSuggestedRecipes(UserId user,
                                                                const std::vector<StorageId>& storages,
                                                                std::size_t size,
                                                                std::size_t offset) const {
    httplib::Params params = {{"size", utils::to_string(size)}, {"offset", std::to_string(offset)}};
    for (const StorageId& id : storages)
        params.emplace("storage-id", utils::to_string(id));
    return jsonGetAuthed<RecipesListWithIngredientsCount>(user, "/suggested-recipes", params);
}

// GET /recipes
RecipeSearchResponse RecipesApi::search(UserId user,
                                        PublicityFilterType filter,
                                        std::string query,
                                        std::size_t threshold,
                                        std::size_t size,
                                        std::size_t offset) const {
    return jsonGetAuthed<RecipeSearchResponse>(user,
                                               "/recipes",
                                               {{"query", std::move(query)},
                                                {"threshold", utils::to_string(threshold)},
                                                {"size", utils::to_string(size)},
                                                {"offset", utils::to_string(offset)},
                                                {"filter", utils::to_string(filter)}});
}

// GET /recipes
RecipesList RecipesApi::getList(UserId user, PublicityFilterType filter, std::size_t size, std::size_t offset) const {
    auto result = search(user, filter, "", 0, size, offset);
    return {.page = std::move(result.page), .found = result.found};
}

// GET /recipes/{recipeId}
RecipeDetails RecipesApi::get(UserId user, RecipeId recipe) const {
    return jsonGetAuthed<RecipeDetails>(user, std::format("/recipes/{}", recipe));
}

// POST /recipes
RecipeId RecipesApi::create(UserId user, const RecipeCreateBody& body) const {
    return utils::parse<RecipeId>(postWithJsonAuthed(user, "/recipes", body));
}

// DELETE /recipes/{recipeId}
void RecipesApi::delete_(UserId user, RecipeId recipeId) const {
    jsonDeleteAuthed<void>(user, std::format("/recipes/{}", recipeId));
}

// POST /recipes/{recipeId}/publication-requests
void RecipesApi::publishCustom(UserId user, RecipeId recipe) const {
    jsonPostAuthed<void>(user, std::format("recipes/{}/publication-requests", recipe));
}

// GET /recipes/{recipeId}/publication-requests
std::vector<RecipePublicationRequest> RecipesApi::getRecipeRequestHistory(UserId user, RecipeId recipe) const {
    return jsonGetAuthed<std::vector<RecipePublicationRequest>>(
        user, std::format("/recipes/{}/publication-requests", recipe));
}

} // namespace cookcookhnya::api
