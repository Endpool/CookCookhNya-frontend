#include "backend/api/recipes.hpp"

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

RecipesList
RecipesApi::getRecipeList(UserId user, size_t size, size_t offset, const std::vector<StorageId>& storages) const {
    httplib::Params params = {{"size", utils::to_string(size)}, {"offset", std::to_string(offset)}};
    for (auto id : storages)
        params.insert({"storageId", utils::to_string(id)});
    return jsonGetAuthed<RecipesList>(user, "/recipes", params);
}

RecipeDetails RecipesApi::getIngredientsInRecipe(UserId user, RecipeId recipe) const {
    return jsonGetAuthed<RecipeDetails>(user, std::format("/recipes/{}", recipe));
}

CustomRecipesList RecipesApi::getPrivateRecipeList(UserId userId, size_t size, size_t offset) const {
    const httplib::Params params = {{"size", utils::to_string(size)}, {"offset", std::to_string(offset)}};
    return jsonGetAuthed<CustomRecipesList>(userId, "/recipes", params);
}

RecipeId RecipesApi::create(UserId user, const RecipeCreateBody& body) const {
    return utils::parse<RecipeId>(postWithJsonAuthed(user, "/recipes", body));
}

void RecipesApi::delete_(UserId user, RecipeId recipeId) const {
    jsonDeleteAuthed<void>(user, std::format("/recipes/{}", recipeId));
}

CustomRecipeDetails RecipesApi::get(UserId user, RecipeId recipe) const {
    return jsonGetAuthed<CustomRecipeDetails>(user, std::format("/recipes/{}", recipe));
}

void RecipesApi::publishRecipe(UserId user, RecipeId recipe) const {
    jsonPostAuthed<void>(user, std::format("my/recipes/{}/publish", recipe));
}

} // namespace cookcookhnya::api
