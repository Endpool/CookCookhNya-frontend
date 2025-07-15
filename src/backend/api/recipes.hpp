#pragma once

#include "backend/api/base.hpp"
#include "backend/id_types.hpp"
#include "backend/models/recipe.hpp"

#include <httplib.h>

#include <cstddef>
#include <vector>

namespace cookcookhnya::api {

class RecipesApi : ApiBase {
    friend class ApiClient;

    explicit RecipesApi(httplib::Client& api) : ApiBase{api} {}

  public:
    [[nodiscard]] models::recipe::RecipesList
    getRecipeList(UserId user, size_t size, size_t offset, const std::vector<StorageId>& storages) const;

    [[nodiscard]] models::recipe::RecipeDetails getIngredientsInRecipe(UserId user, RecipeId recipe) const;

    [[nodiscard]] models::recipe::RecipeSummary getRecipeName(UserId user, RecipeId recipe) const;

    [[nodiscard]] models::recipe::CustomRecipesList
    getPrivateRecipeList(UserId user, size_t size, size_t offset, std::string filter = "custom")
        const; // Set default variable to keep code distinct for personal account instances

    [[nodiscard]] RecipeId create(UserId user, // NOLINT(*-nodiscard)
                                  const models::recipe::RecipeCreateBody& body) const;

    void delete_(UserId user, RecipeId recipe) const;

    [[nodiscard]] models::recipe::CustomRecipeDetails get(UserId user, RecipeId recipe) const;

    void publishRecipe(UserId user, RecipeId recipe) const;
};

} // namespace cookcookhnya::api
