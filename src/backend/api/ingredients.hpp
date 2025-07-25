#pragma once

#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"
#include "base.hpp"
#include "publicity_filter.hpp"

#include <httplib.h>

#include <cstddef>
#include <string>
#include <vector>

namespace cookcookhnya::api {

class IngredientsApi : ApiBase {
    friend class ApiClient;

    explicit IngredientsApi(httplib::Client& api) : ApiBase{api} {}

  public:
    [[nodiscard]] models::ingredient::Ingredient get(UserId user, IngredientId ingredient) const;

    [[nodiscard]] std::vector<models::ingredient::Ingredient>
    getStorageIngredients(UserId user,
                          StorageId storage,
                          std::size_t count = 200, // NOLINT(*magic-number*)
                          std::size_t offset = 0) const;

    void putToStorage(UserId user, StorageId storage, IngredientId ingredient) const;
    void deleteFromStorage(UserId user, StorageId storage, IngredientId ingredient) const;

    void deleteMultipleFromStorage(UserId user, StorageId storage, const std::vector<IngredientId>& ingredients) const;

    [[nodiscard]] models::ingredient::IngredientSearchForStorageResponse
    searchForStorage(UserId user,
                     StorageId storage,
                     std::string query = "",
                     std::size_t count = 50, // NOLINT(*magic-number*)
                     std::size_t offset = 0,
                     unsigned threshold = 50) const; // NOLINT(*magic-number*)

    [[nodiscard]] models::ingredient::IngredientSearchResponse
    search(UserId user,
           PublicityFilterType filter = PublicityFilterType::All,
           std::string query = "",
           std::size_t count = 50, // NOLINT(*magic-number*)
           std::size_t offset = 0,
           unsigned threshold = 50) const; // NOLINT(*magic-number*)

    [[nodiscard]] models::ingredient::IngredientList getList(UserId user,
                                                             PublicityFilterType filter = PublicityFilterType::All,
                                                             std::size_t count = 50, // NOLINT(*magic-number*)
                                                             std::size_t offset = 0) const;

    [[nodiscard]] models::ingredient::CustomIngredientList
    customIngredientsSearch(UserId user,
                            std::string query,
                            std::size_t threshold,
                            std::size_t count = 50, // NOLINT(*magic-number*)
                            std::size_t offset = 0) const;

    [[nodiscard]] models::ingredient::Ingredient getPublicIngredient(IngredientId ingredient) const;

    void putToRecipe(UserId user, RecipeId recipeId, IngredientId ingredient) const;
    void deleteFromRecipe(UserId user, RecipeId recipeId, IngredientId ingredient) const;

    [[nodiscard]] models::ingredient::IngredientSearchForRecipeResponse
    searchForRecipe(UserId user,
                    RecipeId recipe,
                    std::string query = "",
                    std::size_t count = 50, // NOLINT(*magic-number*)
                    std::size_t offset = 0,
                    unsigned threshold = 50) const; // NOLINT(*magic-number*)

    IngredientId createCustom(UserId user, // NOLINT(*-nodiscard)
                              const models::ingredient::IngredientCreateBody& body) const;
    void deleteCustom(UserId user, IngredientId ingredient) const;
    void publishCustom(UserId user, IngredientId ingredient) const;
};

using IngredientsApiRef = const api::IngredientsApi&;

} // namespace cookcookhnya::api
