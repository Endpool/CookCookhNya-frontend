#pragma once

#include "backend/api/base.hpp"
#include "backend/id_types.hpp"
#include "backend/models/recipe.hpp"
#include "common.hpp"

#include <httplib.h>

#include <cstddef>
#include <vector>

namespace cookcookhnya::api {

class RecipesApi : ApiBase {
    friend class ApiClient;

    explicit RecipesApi(httplib::Client& api) : ApiBase{api} {}

  public:
    [[nodiscard]] models::recipe::RecipesList
    getSuggestedRecipesList(UserId user, size_t size, size_t offset, const std::vector<StorageId>& storages) const;

    [[nodiscard]] models::recipe::RecipeSearchResponse getRecipesList(UserId user,
                                                                      std::string query,
                                                                      std::size_t threshold,
                                                                      std::size_t size,
                                                                      std::size_t offset,
                                                                      filterType filter) const;

    [[nodiscard]] models::recipe::RecipeDetails getIngredientsInRecipe(UserId user, RecipeId recipe) const;

    [[nodiscard]] models::recipe::RecipeSummary getRecipeName(UserId user, RecipeId recipe) const;

    [[nodiscard]] models::recipe::CustomRecipesList getPrivateRecipesList(
        UserId user, std::string query, std::size_t threshold, std::size_t size, std::size_t offset) const;

    RecipeId create(UserId user, // NOLINT(*-nodiscard)
                    const models::recipe::RecipeCreateBody& body) const;

    void delete_(UserId user, RecipeId recipe) const;

    [[nodiscard]] models::recipe::CustomRecipeDetails get(UserId user, RecipeId recipe) const;

    void publishRecipe(UserId user, RecipeId recipe) const;
};

} // namespace cookcookhnya::api
