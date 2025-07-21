#pragma once

#include "ingredients.hpp"
#include "moderation.hpp"
#include "recipes.hpp"
#include "shopping_lists.hpp"
#include "storages.hpp"
#include "users.hpp"

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
    ModerationApi moderation;

  public:
    explicit ApiClient(const std::string& apiAddress)
        : api{apiAddress}, users{api}, storages{api}, ingredients{api}, recipes{api}, shoppingList{api},
          moderation{api} {}
    ApiClient(const ApiClient&) = delete;
    ApiClient(ApiClient&& other) noexcept
        : api{std::move(other.api)}, users{api}, storages{api}, ingredients{api}, recipes{api}, shoppingList{api},
          moderation{api} {}
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
        moderation = ModerationApi{api};
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

    [[nodiscard]] const ModerationApi& getModerationApi() const {
        return moderation;
    }

    operator const ModerationApi&() const { // NOLINT(*-explicit-*)
        return moderation;
    }
};

using ApiClientRef = const api::ApiClient&;

} // namespace cookcookhnya::api
