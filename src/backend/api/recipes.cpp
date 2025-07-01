#include "backend/api/recipes.hpp"

#include "backend/id_types.hpp"
#include "backend/models/recipe.hpp"

#include <httplib.h>

#include <format>
#include <string>
#include <vector>

namespace cookcookhnya::api {

using namespace models::recipe;

RecipesList
RecipesApi::getRecipeList(UserId userId, int size, int offset, const std::vector<StorageId>& storageIds) const {
    httplib::Params params = {{"size", std::to_string(size)}, {"offset", std::to_string(offset)}};
    for (auto id : storageIds)
        params.insert({"storageId", std::to_string(id)});
    return jsonGetAuthed<RecipesList>(userId, "/recipes", params);
}

RecipeDetails
RecipesApi::getIngredientsInRecipe(UserId userId, RecipeId recipeId, const std::vector<StorageId>& storageIds) const {
    httplib::Params params;
    for (auto id : storageIds)
        params.insert({"storageId", std::to_string(id)});
    return jsonGetAuthed<RecipeDetails>(userId, std::format("/recipes/{}", recipeId), params);
}

} // namespace cookcookhnya::api
