#pragma once

#include "backend/api/base.hpp"
#include "backend/models/storage.hpp"

#include <httplib.h>

#include <vector>

namespace cookcookhnya::api {

class StoragesApi : ApiBase {
    friend class ApiClient;

    explicit StoragesApi(httplib::Client& api);

  public:
    [[nodiscard]] std::vector<models::storage::StorageSummary> getStoragesList() const;
};

} // namespace cookcookhnya::api
