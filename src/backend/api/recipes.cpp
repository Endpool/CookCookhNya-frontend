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
RecipesApi::getRecipeList(UserId user, size_t size, size_t offset, const std::vector<StorageId>& storages) const {
    httplib::Params params = {{"size", std::to_string(size)}, {"offset", std::to_string(offset)}};
    for (auto id : storages)
        params.insert({"storageId", std::to_string(id)});
    return jsonGetAuthed<RecipesList>(user, "/recipes", params);
}

RecipeDetails RecipesApi::getIngredientsInRecipe(UserId user, RecipeId recipe)
    const { // StorageIds are not needed in current implementation as backend doesnt return if ingredient is available
    return jsonGetAuthed<RecipeDetails>(user, std::format("/recipes/{}", recipe));
}

CustomRecipesList RecipesApi::getPrivateRecipeList(UserId user, size_t size, size_t offset) const {
    const httplib::Params params = {{"size", std::to_string(size)}, {"offset", std::to_string(offset)}};
    return jsonGetAuthed<CustomRecipesList>(user, "/recipes", params);
}

RecipeId RecipesApi::create(UserId user, const RecipeCreateBody& body) const {
    return jsonPostWithJsonAuthed<RecipeId>(user, "/my/recipes", body); // path analogically to storages
}

void RecipesApi::delete_(UserId user, RecipeId recipe) const {
    jsonDeleteAuthed<void>(user, std::format("/my/recipes/{}", recipe)); // path analogically to storages
}

CustomRecipeDetails RecipesApi::get(UserId user, RecipeId recipe) const {
    return jsonGetAuthed<CustomRecipeDetails>(user,
                                              std::format("/my/recipes/{}", recipe)); // path analogically to storages
}

void RecipesApi::publishRecipe(UserId user, RecipeId recipe) const {
    jsonPostAuthed<void>(user, std::format("my/recipes/{}/publish", recipe));
}

} // namespace cookcookhnya::api
