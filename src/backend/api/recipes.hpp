#pragma once

#include "backend/api/base.hpp"
#include "backend/id_types.hpp"
#include "backend/models/recipe.hpp"

#include <cstddef>
#include <httplib.h>

#include <vector>

namespace cookcookhnya::api {

class RecipesApi : ApiBase {
    friend class ApiClient;

    explicit RecipesApi(httplib::Client& api) : ApiBase{api} {}

  public:
    [[nodiscard]] models::recipe::RecipesList
    getRecipeList(UserId userId, size_t size, size_t offset, const std::vector<StorageId>& storageIds) const;
    [[nodiscard]] models::recipe::RecipeDetails getIngredientsInRecipe(UserId userId, RecipeId recipeId) const;
    [[nodiscard]] models::recipe::RecipeSummary getRecipeName(UserId userId, RecipeId recipeId) const;
    [[nodiscard]] models::recipe::CustomRecipesList
    getPrivateRecipeList(UserId userId, size_t size, size_t offset) const;
    RecipeId create(UserId userId, // NOLINT(*-nodiscard)
                    const models::recipe::RecipeCreateBody& body) const;
    void delete_(UserId userId, RecipeId id) const;
    [[nodiscard]] models::recipe::CustomRecipeDetails get(UserId userId, RecipeId id) const;
    void publishRecipe(UserId userId, RecipeId recipeId) const;
};

} // namespace cookcookhnya::api
