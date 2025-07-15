#pragma once

#include "backend/api/base.hpp"
#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"
#include "common.hpp"

#include <httplib.h>

#include <cstddef>
#include <vector>

namespace cookcookhnya::api {

class IngredientsApi : ApiBase {
    friend class ApiClient;

    explicit IngredientsApi(httplib::Client& api) : ApiBase{api} {}

  public:
    [[nodiscard]] std::vector<models::ingredient::Ingredient> getStorageIngredients(UserId user,
                                                                                    StorageId storage) const;

    void putToStorage(UserId user, StorageId storage, IngredientId ingredient) const;

    void deleteFromStorage(UserId user, StorageId storage, IngredientId ingredient) const;

    void deleteMultipleFromStorage(UserId user, StorageId storage, const std::vector<IngredientId>& ingredients) const;

    [[nodiscard]] models::ingredient::IngredientSearchForStorageResponse
    searchForStorage(UserId user, std::string query, StorageId storage, std::size_t count, std::size_t offset) const;

    [[nodiscard]] models::ingredient::IngredientSearchResponse
    publicSearch(std::string query, std::size_t size, std::size_t offset, std::size_t threshold) const;

    [[nodiscard]] models::ingredient::IngredientSearchResponse search(UserId user,
                                                                      std::string query,
                                                                      std::size_t threshold,
                                                                      std::size_t size,
                                                                      std::size_t offset,
                                                                      filterType filter) const;

    [[nodiscard]] models::ingredient::Ingredient getPublicIngredient(IngredientId ingredient) const;

    void putToRecipe(UserId user, RecipeId recipeId, IngredientId ingredient) const;

    void deleteFromRecipe(UserId user, RecipeId recipeId, IngredientId ingredient) const;

    [[nodiscard]] models::ingredient::IngredientSearchForRecipeResponse
    searchForRecipe(UserId user, std::string query, RecipeId recipeId, std::size_t count, std::size_t offset) const;

    IngredientId createCustom(UserId user, // NOLINT(*-nodiscard)
                              const models::ingredient::IngredientCreateBody& body) const;

    void publishCustom(UserId user, IngredientId ingredient) const;
};

} // namespace cookcookhnya::api
