#include "backend/api/recipes.hpp"

#include "backend/models/recipe.hpp"

#include <string>
#include <vector>

namespace cookcookhnya::api {

using namespace models::recipe;

RecipesList
RecipesApi::getRecipeList(UserId userId, int size, int offset, std::vector<StorageId> const& storageIds) const {
    std::multimap<std::string, std::string> multimap = {{"size", std::to_string(size)},
                                                        {"offset", std::to_string(offset)}};
    for (auto temp : storageIds) {
        multimap.insert({"storageId", std::to_string(temp)});
    }
    return jsonGetAuthed<RecipesList>(userId, "/recipes", multimap);
}

RecipeDetails RecipesApi::getIngredientsInRecipe(UserId userId, RecipeId recipeId)
    const { // StorageIds are not needed in current implementation as backend doesnt return if ingredient is available
    return jsonGetAuthed<RecipeDetails>(userId, std::format("/recipes/{}", recipeId));
}

} // namespace cookcookhnya::api
