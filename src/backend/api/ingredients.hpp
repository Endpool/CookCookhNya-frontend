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
    [[nodiscard]] std::vector<models::ingredient::Ingredient> getStorageIngredients(UserId user,
                                                                                    StorageId storage) const;
    void putToStorage(UserId user, StorageId storage, IngredientId id) const;

    void deleteFromStorage(UserId user, StorageId storage, IngredientId id) const;

    [[nodiscard]] std::vector<models::ingredient::Ingredient> getAllIngredients() const;

    [[nodiscard]] models::ingredient::Ingredient get(IngredientId id) const;

    [[nodiscard]] models::ingredient::IngredientSearchResponse
    searchForStorage(UserId user, std::string query, StorageId storage, std::size_t count, std::size_t offset) const;

    void putToRecipe(UserId user, RecipeId recipeId, IngredientId id) const;
    [[nodiscard]] models::ingredient::IngredientSearchResponse
    searchForRecipe(UserId user, std::string query, RecipeId recipeId, std::size_t count, std::size_t offset) const;
    void deleteFromRecipe(UserId user, RecipeId recipeId, IngredientId id) const;
};

} // namespace cookcookhnya::api
