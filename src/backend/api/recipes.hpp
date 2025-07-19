#pragma once

#include "backend/api/base.hpp"
#include "backend/id_types.hpp"
#include "backend/models/recipe.hpp"
#include "common.hpp"

#include <httplib.h>

#include <cstddef>
#include <string>
#include <vector>

namespace cookcookhnya::api {

class RecipesApi : ApiBase {
    friend class ApiClient;

    explicit RecipesApi(httplib::Client& api) : ApiBase{api} {}

  public:
    [[nodiscard]] models::recipe::RecipesList getSuggestedRecipesList(UserId user,
                                                                      const std::vector<StorageId>& storages,
                                                                      size_t size = 2,
                                                                      size_t offset = 0) const;

    [[nodiscard]] models::recipe::RecipeSearchResponse getRecipesList(UserId user,
                                                                      std::string query,
                                                                      std::size_t threshold,
                                                                      std::size_t size,
                                                                      std::size_t offset,
                                                                      filterType filter) const;

    [[nodiscard]] models::recipe::RecipeDetails getIngredientsInRecipe(UserId user, RecipeId recipe) const;

    [[nodiscard]] RecipeId create(UserId user, // NOLINT(*-nodiscard)
                                  const models::recipe::RecipeCreateBody& body) const;
    [[nodiscard]] std::vector<models::recipe::CustomRecipePublication> getModerationHistory(UserId user,
                                                                                            RecipeId recipe) const;
    void delete_(UserId user, RecipeId recipe) const;

    [[nodiscard]] models::recipe::CustomRecipeDetails get(UserId user, RecipeId recipe) const;

    void publishCustom(UserId user, RecipeId recipe) const;
};

} // namespace cookcookhnya::api
