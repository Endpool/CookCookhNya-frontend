#pragma once

#include "backend/id_types.hpp"
#include "backend/models/recipe.hpp"
#include "base.hpp"
#include "publicity_filter.hpp"

#include <httplib.h>

#include <cstddef>
#include <string>
#include <vector>

namespace cookcookhnya::api {

class RecipesApi : ApiBase {
    friend class ApiClient;

    explicit RecipesApi(httplib::Client& api) : ApiBase{api} {}

  public:
    [[nodiscard]] models::recipe::RecipesListWithIngredientsCount
    getSuggestedRecipes(UserId user,
                        const std::vector<StorageId>& storages,
                        size_t size = 500, // NOLINT(*magic-number*)
                        size_t offset = 0) const;

    [[nodiscard]] models::recipe::RecipeSearchResponse
    search(UserId user,
           PublicityFilterType filter = PublicityFilterType::All,
           std::string query = "",
           std::size_t size = 100, // NOLINT(*magic-number*)
           std::size_t offset = 0,
           std::size_t threshold = 50) const; // NOLINT(*magic-number*)

    [[nodiscard]] models::recipe::RecipesList getList(UserId user,
                                                      PublicityFilterType filter = PublicityFilterType::All,
                                                      std::size_t size = 100, // NOLINT(*magic-number*)
                                                      std::size_t offset = 0) const;

    RecipeId create(UserId user, // NOLINT(*-nodiscard)
                    const models::recipe::RecipeCreateBody& body) const;
    [[nodiscard]] std::vector<models::recipe::RecipePublicationRequest> getRecipeRequestHistory(UserId user,
                                                                                                RecipeId recipe) const;

    [[nodiscard]] models::recipe::SuggestedRecipeDetails getSuggested(UserId user, RecipeId recipeId) const;

    void delete_(UserId user, RecipeId recipe) const;

    void publishCustom(UserId user, RecipeId recipe) const;
};

using RecipesApiRef = const api::RecipesApi&;

} // namespace cookcookhnya::api
