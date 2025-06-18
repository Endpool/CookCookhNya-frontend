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
    [[nodiscard]] models::storage::StorageDetails get(models::storage::StorageId id) const;
};

} // namespace cookcookhnya::api
