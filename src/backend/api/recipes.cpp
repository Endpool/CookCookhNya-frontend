#include "backend/api/recipes.hpp"

#include "backend/api/common.hpp"
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
RecipesList RecipesApi::getSuggestedRecipesList(UserId user,
                                                const std::vector<StorageId>& storages,
                                                size_t size,
                                                size_t offset) const {
    httplib::Params params = {{"size", utils::to_string(size)}, {"offset", std::to_string(offset)}};
    for (auto id : storages)
        params.insert({"storageId", utils::to_string(id)});
    return jsonGetAuthed<RecipesList>(user, "/suggested-recipes", params);
}

// GET /recipes
RecipeSearchResponse RecipesApi::getRecipesList(UserId user,
                                                std::string query,
                                                std::size_t threshold,
                                                std::size_t size,
                                                std::size_t offset,
                                                filterType filter) const {
    return jsonGetAuthed<RecipeSearchResponse>(user,
                                               "/recipes",
                                               {{"query", std::move(query)},
                                                {"threshold", utils::to_string(threshold)},
                                                {"size", utils::to_string(size)},
                                                {"offset", utils::to_string(offset)},
                                                {"filter", filterStr(filter)}});
}

// GET /recipes/{recipeId}
RecipeDetails RecipesApi::getIngredientsInRecipe(UserId user, RecipeId recipe) const {
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

// GET /recipes/{recipeId}
CustomRecipeDetails RecipesApi::get(UserId user, RecipeId recipe) const {
    return jsonGetAuthed<CustomRecipeDetails>(user, std::format("/recipes/{}", recipe));
}

// POST /recipes/{recipeId}/request-publication
void RecipesApi::publishCustom(UserId user, RecipeId recipe) const {
    jsonPostAuthed<void>(user, std::format("my/recipes/{}/request-publication", recipe));
}

} // namespace cookcookhnya::api
