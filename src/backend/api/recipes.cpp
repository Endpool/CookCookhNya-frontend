#include "backend/api/recipes.hpp"

#include "backend/id_types.hpp"
#include "backend/models/recipe.hpp"

#include <cstddef>
#include <httplib.h>

#include <format>
#include <string>
#include <vector>

namespace cookcookhnya::api {

using namespace models::recipe;

RecipesList
RecipesApi::getRecipeList(UserId userId, size_t size, size_t offset, const std::vector<StorageId>& storageIds) const {
    httplib::Params params = {{"size", std::to_string(size)}, {"offset", std::to_string(offset)}};
    for (auto id : storageIds)
        params.insert({"storageId", std::to_string(id)});
    return jsonGetAuthed<RecipesList>(userId, "/recipes", params);
}

RecipeDetails RecipesApi::getIngredientsInRecipe(UserId userId, RecipeId recipeId)
    const { // StorageIds are not needed in current implementation as backend doesnt return if ingredient is available
    return jsonGetAuthed<RecipeDetails>(userId, std::format("/recipes/{}", recipeId));
}

CustomRecipesList RecipesApi::getPrivateRecipeList(UserId userId, size_t size, size_t offset) const {
    httplib::Params params = {{"size", std::to_string(size)}, {"offset", std::to_string(offset)}};
    return jsonGetAuthed<CustomRecipesList>(userId, "/recipes", params);
}

RecipeId RecipesApi::create(UserId userId, const RecipeCreateBody& body) const {
    return jsonPostWithJsonAuthed<RecipeId>(userId, "/my/recipes", body); // path analogically to storages
}

void RecipesApi::delete_(UserId userId, RecipeId recipeId) const {
    jsonDeleteAuthed<void>(userId, std::format("/my/recipes/{}", recipeId)); // path analogically to storages
}

CustomRecipeDetails RecipesApi::get(UserId userId, RecipeId recipeId) const {
    return jsonGetAuthed<CustomRecipeDetails>(userId,
                                              std::format("/my/recipes/{}", recipeId)); // path analogically to storages
}

void RecipesApi::publishRecipe(UserId userId, RecipeId recipeId) const {
    jsonPostAuthed<void>(userId, std::format("my/recipes/{}/publish", recipeId));
}

} // namespace cookcookhnya::api
