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
#include <vector>

namespace cookcookhnya::api {

using namespace models::recipe;

// /suggested-recipes
RecipesList RecipesApi::getSuggestedRecipesList(UserId user,
                                                size_t size,
                                                size_t offset,
                                                const std::vector<StorageId>& storages) const {
    httplib::Params params = {{"size", utils::to_string(size)}, {"offset", std::to_string(offset)}};
    for (auto id : storages)
        params.insert({"storageId", utils::to_string(id)});
    return jsonGetAuthed<RecipesList>(user, "/suggested-recipes", params);
}

// /recipes
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

// /recipes
RecipeDetails RecipesApi::getIngredientsInRecipe(UserId user, RecipeId recipe) const {
    return jsonGetAuthed<RecipeDetails>(user, std::format("/recipes/{}", recipe));
}

// /recipes
CustomRecipesList RecipesApi::getPrivateRecipesList(
    UserId user, std::string query, std::size_t threshold, std::size_t size, std::size_t offset) const {
    return jsonGetAuthed<CustomRecipesList>(user,
                                            "/recipes",
                                            {{"query", std::move(query)},
                                             {"threshold", utils::to_string(threshold)},
                                             {"size", utils::to_string(size)},
                                             {"offset", utils::to_string(offset)},
                                             {"filter", filterStr(filterType::Custom)}});
}

// /recipes
RecipeId RecipesApi::create(UserId user, const RecipeCreateBody& body) const {
    return utils::parse<RecipeId>(postWithJsonAuthed(user, "/recipes", body));
}

// /recipes/{recipeId}
void RecipesApi::delete_(UserId user, RecipeId recipeId) const {
    jsonDeleteAuthed<void>(user, std::format("/recipes/{}", recipeId));
}

// /recipes/{recipeId}
CustomRecipeDetails RecipesApi::get(UserId user, RecipeId recipe) const {
    return jsonGetAuthed<CustomRecipeDetails>(user, std::format("/recipes/{}", recipe));
}

// /recipes/{recipeId}/request-publication
void RecipesApi::publishRecipe(UserId user, RecipeId recipe) const {
    jsonPostAuthed<void>(user, std::format("my/recipes/{}/request-publication", recipe));
}

} // namespace cookcookhnya::api
