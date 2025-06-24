#pragma once

#include "backend/api/ingredients.hpp"
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

  public:
    explicit ApiClient(const std::string& apiAddress) : api{apiAddress}, users{api}, storages{api}, ingredients{api} {}
    ApiClient(const ApiClient&) = delete;
    ApiClient(ApiClient&& other) noexcept : api{std::move(other.api)}, users{api}, storages{api}, ingredients{api} {}
    ApiClient& operator=(const ApiClient&) = delete;
    ApiClient& operator=(ApiClient&& other) noexcept {
        if (&other == this)
            return *this;
        api = std::move(other.api);
        users = UsersApi{api};
        storages = StoragesApi{api};
        ingredients = IngredientsApi{api};
        return *this;
    }
    ~ApiClient() = default;

    [[nodiscard]] const UsersApi& getUsers() const {
        return users;
    }

    operator const UsersApi&() const { // NOLINT(*-explicit-*)
        return users;
    }

    [[nodiscard]] const StoragesApi& getStorages() const {
        return storages;
    }

    operator const StoragesApi&() const { // NOLINT(*-explicit-*)
        return storages;
    }

    [[nodiscard]] const IngredientsApi& getIngredients() const {
        return ingredients;
    }

    operator const IngredientsApi&() const { // NOLINT(*-explicit-*)
        return ingredients;
    }
};

} // namespace cookcookhnya::api
