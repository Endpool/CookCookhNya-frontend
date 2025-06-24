#include "backend/api/recipes.hpp"

#include "backend/models/recipe.hpp"

#include <string>
#include <vector>

namespace cookcookhnya::api {

using namespace models::recipe;

RecipesSummary RecipesApi::getRecipeList(UserId userId, int size, int offset, std::vector<StorageId> storageId) const {
    std::multimap<std::string, std::string> multimap = {{"size", std::to_string(size)},
                                                        {"offset", std::to_string(offset)}};
    for (auto temp : storageId) {
        multimap.insert({"storageId", std::to_string(temp)});
    }
    return jsonGetAuthed<RecipesSummary>(userId, "/recipes", multimap);
}

} // namespace cookcookhnya::api
