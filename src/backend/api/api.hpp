#pragma once

#include "backend/api/ingredients.hpp"
#include "backend/api/recipes.hpp"
#include "backend/api/shopping_lists.hpp"
#include "backend/api/storages.hpp"
#include "backend/api/users.hpp"

#include <httplib.h>

#include <string>
#include <utility>

namespace cookcookhnya::api {

class ApiClient {
    httplib::Client api;
    UsersApi users;
    StoragesApi storages;
    IngredientsApi ingredients;
    RecipesApi recipes;
    ShoppingListApi shoppingList;

  public:
    explicit ApiClient(const std::string& apiAddress)
        : api{apiAddress}, users{api}, storages{api}, ingredients{api}, recipes{api}, shoppingList{api} {}
    ApiClient(const ApiClient&) = delete;
    ApiClient(ApiClient&& other) noexcept
        : api{std::move(other.api)}, users{api}, storages{api}, ingredients{api}, recipes{api}, shoppingList{api} {}
    ApiClient& operator=(const ApiClient&) = delete;
    ApiClient& operator=(ApiClient&& other) noexcept {
        if (&other == this)
            return *this;
        api = std::move(other.api);
        users = UsersApi{api};
        storages = StoragesApi{api};
        ingredients = IngredientsApi{api};
        recipes = RecipesApi{api};
        shoppingList = ShoppingListApi{api};
        return *this;
    }
    ~ApiClient() = default;

    [[nodiscard]] const UsersApi& getUsersApi() const {
        return users;
    }

    operator const UsersApi&() const { // NOLINT(*-explicit-*)
        return users;
    }

    [[nodiscard]] const StoragesApi& getStoragesApi() const {
        return storages;
    }

    operator const StoragesApi&() const { // NOLINT(*-explicit-*)
        return storages;
    }

    [[nodiscard]] const IngredientsApi& getIngredientsApi() const {
        return ingredients;
    }

    operator const IngredientsApi&() const { // NOLINT(*-explicit-*)
        return ingredients;
    }

    [[nodiscard]] const RecipesApi& getRecipesApi() const {
        return recipes;
    }

    operator const RecipesApi&() const { // NOLINT(*-explicit-*)
        return recipes;
    }

    [[nodiscard]] const ShoppingListApi& getShoppingListApi() const {
        return shoppingList;
    }

    operator const ShoppingListApi&() const { // NOLINT(*-explicit-*)
        return shoppingList;
    }
};

} // namespace cookcookhnya::api
