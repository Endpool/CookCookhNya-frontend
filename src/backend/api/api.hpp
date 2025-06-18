#pragma once

#include "backend/api/storages.hpp"

#include <httplib.h>

#include <string>

namespace cookcookhnya::api {

class ApiClient {
    httplib::Client api;
    StoragesApi storages;

  public:
    explicit ApiClient(const std::string& apiAddress) : api{apiAddress}, storages{api} {}

    [[nodiscard]] const StoragesApi& getStorages() const {
        return storages;
    }

    operator const StoragesApi&() const { // NOLINT(*-explicit-*)
        return storages;
    }
};

} // namespace cookcookhnya::api
