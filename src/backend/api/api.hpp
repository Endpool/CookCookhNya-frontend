#pragma once

#include "backend/api/storages.hpp"
#include "backend/api/users.hpp"

#include <httplib.h>

#include <string>
#include <utility>

namespace cookcookhnya::api {

class ApiClient {
    httplib::Client api;
    StoragesApi storages;
    UsersApi users;

  public:
    explicit ApiClient(const std::string& apiAddress) : api{apiAddress}, storages{api}, users{api} {}
    ApiClient(const ApiClient&) = delete;
    ApiClient(ApiClient&& other) noexcept : api{std::move(other.api)}, storages{api}, users{api} {}
    ApiClient& operator=(const ApiClient&) = delete;
    ApiClient& operator=(ApiClient&& other) noexcept {
        if (&other == this)
            return *this;
        api = std::move(other.api);
        storages = StoragesApi{api};
        users = UsersApi{api};
        return *this;
    }
    ~ApiClient() = default;

    [[nodiscard]] const StoragesApi& getStorages() const {
        return storages;
    }

    operator const StoragesApi&() const { // NOLINT(*-explicit-*)
        return storages;
    }

    [[nodiscard]] const UsersApi& getUsers() const {
        return users;
    }

    operator const UsersApi&() const { // NOLINT(*-explicit-*)
        return users;
    }
};

} // namespace cookcookhnya::api
