#pragma once

#include "backend/api/storages.hpp"

#include <httplib.h>

#include <string>
#include <utility>

namespace cookcookhnya::api {

class ApiClient {
    httplib::Client api;
    StoragesApi storages;

  public:
    explicit ApiClient(const std::string& apiAddress) : api{apiAddress}, storages{api} {}
    ApiClient(const ApiClient&) = delete;
    ApiClient(ApiClient&& other) noexcept : api{std::move(other.api)}, storages{api} {}
    ApiClient& operator=(const ApiClient&) = delete;
    ApiClient& operator=(ApiClient&& other) noexcept {
        if (&other == this)
            return *this;
        api = std::move(other.api);
        storages = StoragesApi{api};
        return *this;
    }
    ~ApiClient() = default;

    [[nodiscard]] const StoragesApi& getStorages() const {
        return storages;
    }

    operator const StoragesApi&() const { // NOLINT(*-explicit-*)
        return storages;
    }
};

} // namespace cookcookhnya::api
