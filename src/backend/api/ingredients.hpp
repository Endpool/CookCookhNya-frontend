#pragma once

#include "backend/api/base.hpp"
#include "backend/id_types.hpp"
#include "backend/models/ingredient.hpp"

#include <httplib.h>

#include <cstddef>
#include <vector>

namespace cookcookhnya::api {

class IngredientsApi : ApiBase {
    friend class ApiClient;

    explicit IngredientsApi(httplib::Client& api) : ApiBase{api} {}

  public:
    [[nodiscard]] std::vector<models::ingredient::Ingredient> getCustomIngredients(UserId user) const;

    IngredientId createCustom(UserId user, // NOLINT(*-nodiscard)
                              const models::ingredient::IngredientCreateBody& body) const;

    void publishCustom(UserId user, IngredientId ingredient) const;

    [[nodiscard]] std::vector<models::ingredient::Ingredient> getStorageIngredients(UserId user,
                                                                                    StorageId storage) const;
    void putToStorage(UserId user, StorageId storage, IngredientId ingredient) const;

    void deleteFromStorage(UserId user, StorageId storage, IngredientId ingredient) const;

    [[nodiscard]] models::ingredient::Ingredient get(IngredientId ingredient) const;

    [[nodiscard]] models::ingredient::IngredientSearchResponse
    search(std::string query, std::size_t size, std::size_t offset, std::size_t threshold) const;

    [[nodiscard]] models::ingredient::IngredientSearchForStorageResponse
    searchForStorage(UserId user, std::string query, StorageId storage, std::size_t count, std::size_t offset) const;

    void putToRecipe(UserId user, RecipeId recipe, IngredientId ingredient) const;
    [[nodiscard]] models::ingredient::IngredientSearchForRecipeResponse
    searchForRecipe(UserId user, std::string query, RecipeId recipe, std::size_t count, std::size_t offset) const;
    void deleteFromRecipe(UserId user, RecipeId recipe, IngredientId ingredient) const;
};

} // namespace cookcookhnya::api
